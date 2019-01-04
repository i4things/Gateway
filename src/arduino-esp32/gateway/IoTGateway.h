/**
   USE OF THIS SOFTWARE IS GOVERNED BY THE TERMS AND CONDITIONS
   OF THE LICENSE STATEMENT AND LIMITED WARRANTY FURNISHED WITH
   THE PRODUCT.
   <p/>
   IN PARTICULAR, YOU WILL INDEMNIFY AND HOLD B2N LTD., ITS
   RELATED COMPANIES AND ITS SUPPLIERS, HARMLESS FROM AND AGAINST ANY
   CLAIMS OR LIABILITIES ARISING OUT OF THE USE, REPRODUCTION, OR
   DISTRIBUTION OF YOUR PROGRAMS, INCLUDING ANY CLAIMS OR LIABILITIES
   ARISING OUT OF OR RESULTING FROM THE USE, MODIFICATION, OR
   DISTRIBUTION OF PROGRAMS OR FILES CREATED FROM, BASED ON, AND/OR
   DERIVED FROM THIS SOURCE CODE FILE.
*/

//class IoTGateway
//{
//  public:
//    // constructor
//    IoTGateway(uint8_t slaveSelectPin_,
//             uint8_t interruptPin_,
//             uint8_t resetPin_,
//             void (* received_)(uint8_t buf[], uint8_t size, int16_t rssi),
//             uint8_t gateway_key_[16],
//             uint64_t gateway_id_ );
//
//    // call before using
//    void init(float freq);
//
//    // set id ( in case after start needs to be changed)
//    void set_id(uint64_t gateway_id_ );
//
//    // set key ( in case after start needs to be changed)
//    void set_key(uint8_t gateway_key_[16]);
//
//    // set frequency
//    void set_freq(float freq);
//
//    // return signal strength in %
//    uint8_t signal_strength();
//
//    // return current frequency
//    float signal_frequency();
//
//    //return total acknowledged messages send count
//    uint32_t ack_messages();
//
//    //return received messages count for the gateway
//    uint32_t recv_messages();
//
//    //return all received messages count
//    uint32_t all_messages();
//
//    // generate message ready to be sent over internet to the server
//    void gen_internet_message(uint8_t out_buf[], uint8_t & out_size, uint8_t in_buf[], uint8_t in_size, int16_t rssi);
//
//    // call to add message to disptach
//    void add_dispatch_buffer(uint8_t buf_[], uint8_t start_index_, uint8_t size_);
//
//    // clear dispatch buffer
//    void clear_dispatch_buffer();
//
//    // please call in the main loop to be able to dispatch data and menage logic
//    void work();
//};

#include <RH_RF95.h>
#include <SPI.h> // Not actualy used but needed to compile

class RH_RF95_IoTGateway : public RH_RF95
{
  public:
    RH_RF95_IoTGateway(uint8_t slaveSelectPin = SS, uint8_t interruptPin = 2, RHGenericSPI& spi = hardware_spi) : RH_RF95(slaveSelectPin, interruptPin, spi)
    {
    }

    bool isChannelActive()
    {
      _curRssi = spiRead(RH_RF95_REG_2C_RSSI_WIDEBAND);
      if (_curHFport)
        _curRssi -= 157;
      else
        _curRssi -= 164;

      return (_curRssi > -128);
    }

    bool setFrequency(float centre)
    {
      _curHFport = (centre >= 779.0);
      return RH_RF95::setFrequency(centre);
    }

  protected :

    int8_t _curSNR;
    int16_t _curRssi;
    bool _curHFport;
};

#define IoTGateway_MAX_DISPATCH_MSG 60
#define IoTGateway_MIN_RSSI  (-150)
#define IoTGateway_MAX_MESSAGE_LEN 51
#define IoTGateway_ACK_TIMEOUT 400
#define IoTGateway_MAGIC 8606
#define IoTGateway_DISPATCH_TIMEOUT 1000
#define IoTGateway_OPER_DATA 128
#define IoTGateway_SEQ_COUNTER_MIN 2
uint8_t IoTGateway_SEQ_COUNTER = IoTGateway_SEQ_COUNTER_MIN;

class IoTGateway
{
  public :

    IoTGateway(uint8_t slaveSelectPin_,
               uint8_t interruptPin_,
               uint8_t resetPin_,
               void (* received_)(uint8_t buf[], uint8_t size, int16_t rssi),
               uint8_t gateway_key_[16],
               uint64_t gateway_id_ ) :
      resetPin(resetPin_),
      driver(RH_RF95_IoTGateway(slaveSelectPin_, interruptPin_)),
      received(received_),
      gateway_id(gateway_id_)
    {
      memcpy(gateway_key,  gateway_key_, 16);
    }

    // call before using
    void init(float freq_)
    {
      first = false;
      total_ack = 0;
      total_recv = 0;
      total_all = 0;
      rssi = IoTGateway_MIN_RSSI;
      active_start = 0;
      freq = freq_;

      dispatch_last_execute = millis();
      dispatch_buf_size = 0;
      last_index_disatch_buf = 0;
      active_start_dispatch = 0;

      q = NULL;

      pinMode(resetPin, OUTPUT);
      digitalWrite(resetPin, HIGH);

      // manual reset
      digitalWrite(resetPin, LOW);
      delay(10);
      digitalWrite(resetPin, HIGH);
      delay(10);

      if (!driver.init())
      {
#if defined (LOG64_ENABLED)
        LOG64_SET(F("IoTG: FAILED"));
        LOG64_NEW_LINE;
#endif
      }

      if (!driver.setFrequency(freq))
      {
#if defined (LOG64_ENABLED)
        LOG64_SET(F("IoTG: FREQ FAILED"));
        LOG64_NEW_LINE;
#endif
      }

      // max power
      driver.setTxPower(23, false);

      driver.available(); // trurns the driver ON
#if defined (LOG64_ENABLED)
      LOG64_SET(F("IoTG: INIT"));
      LOG64_NEW_LINE;
#endif
    }

  public:
    // set id ( in case after start needs to be changed)
    void set_id(uint64_t gateway_id_ )
    {
      gateway_id = gateway_id_;
    }

    // set key ( in case after start needs to be changed)
    void set_key(uint8_t gateway_key_[16])
    {
      memcpy(gateway_key,  gateway_key_, 16);
    }
  public:

    inline void gen_internet_message(uint8_t out_buf_[], uint8_t & out_size_, uint8_t in_buf_[], uint8_t in_size_, int16_t rssi_)
    {
      uint32_t crc = crc4(in_buf_, in_size_);

      // using out_buf as a temp storage
      memcpy(out_buf_, &crc, 4);
      memcpy(&out_buf_[4], in_buf_, in_size_);
      uint8_t *enc;
      size_t enc_size;

      enc =  xxtea_encrypt(out_buf_, in_size_ + 4, gateway_key,  &enc_size);

      uint16_t magic = 86;
      magic = magic << 8;
      magic += (IoTGateway_SEQ_COUNTER++);
      // 0 is special sequence
      if (IoTGateway_SEQ_COUNTER > 126)
      {
        IoTGateway_SEQ_COUNTER = IoTGateway_SEQ_COUNTER_MIN;
      }

      out_buf_[0] = 9 + enc_size;

      out_buf_[1] = IoTGateway_OPER_DATA;

      out_buf_[2] = ((uint8_t*)&magic)[0];
      out_buf_[3] = ((uint8_t*)&magic)[1];

      out_buf_[4] = ((uint8_t*)&rssi)[0];
      out_buf_[5] = ((uint8_t*)&rssi)[1];

      out_buf_[6] = ((uint8_t*)&gateway_id)[0];
      out_buf_[7] = ((uint8_t*)&gateway_id)[1];
      out_buf_[8] = ((uint8_t*)&gateway_id)[2];
      out_buf_[9] = ((uint8_t*)&gateway_id)[3];

      memcpy(&out_buf_[10], enc, enc_size);

      out_size_ = enc_size + 10;
      free(enc);
    }
  public:
    // call to add message to disptach
    void add_dispatch_buffer(uint8_t buf_[], uint8_t start_index_, uint8_t size_)
    {
      dispatch_buf[dispatch_buf_size++] = size_;
      memcpy(&dispatch_buf[dispatch_buf_size], &buf_[start_index_], size_);
      dispatch_buf_size += size_;
    }

    // clear dispatch buffer
    void clear_dispatch_buffer()
    {
      dispatch_buf_size = 0;
      last_index_disatch_buf = 0;
      active_start_dispatch = 0;
      dispatch_last_execute = millis();
    }

  private:

    inline void ack_dispatch(int8_t seq)
    {
      for (uint16_t i = 0; i < dispatch_buf_size;  )
      {
        if (dispatch_buf[i + 2] == seq)
        {
          // ack set to 1 - to prevent sending again
          dispatch_buf[i + 2] = 1;

          break;
        }
        i += (((uint16_t)dispatch_buf[i]) + 1);
      }
    }

    inline void get_dispatch(uint8_t buf_[], uint8_t & out_size_)
    {
      // init with 0 in case we discover there is no data
      out_size_ = 0;

      if (dispatch_buf_size == 0)
      {
        return;
      }

      for (uint8_t j = 0; j < 2 ; j++)
      {
        // we want it run two time as need to be sure tahtw e have iterated the full buffer
        for (uint16_t i = last_index_disatch_buf; i < dispatch_buf_size;  )
        {
          if (dispatch_buf[i + 2] == 0)
          {
            // init message
            dispatch_buf[i + 2] = (IoTGateway_SEQ_COUNTER++);
            // 0 is special sequence
            if (IoTGateway_SEQ_COUNTER > 126)
            {
              IoTGateway_SEQ_COUNTER = IoTGateway_SEQ_COUNTER_MIN;
            }

            dispatch_buf[i + 1] = crc(dispatch_buf, i + ((uint16_t)dispatch_buf[i]) + 1 , i + 2);

          }
          if (dispatch_buf[i + 2] >= IoTGateway_SEQ_COUNTER_MIN)
          {
            last_index_disatch_buf = i + ((uint16_t)dispatch_buf[i]) + 1;
            out_size_ = last_index_disatch_buf - (i + 1);
            memcpy((void *)buf_, (void *)& dispatch_buf[i + 1], out_size_);
            if (last_index_disatch_buf == dispatch_buf_size)
            {
              last_index_disatch_buf = 0;
            }

            return;
          }
          i += (((uint16_t)dispatch_buf[i]) + 1);
        }
        last_index_disatch_buf = 0;
      }
      // we do not have anything in the buf . next time get out directly until the buff is filled again
      dispatch_buf_size = 0;
    }

  public:
    // set frequency
    inline void set_freq(float freq)
    {
      if (!driver.setFrequency(freq))
      {
#if defined (LOG64_ENABLED)
        LOG64_SET(F("IoTG: FREQ FAILED"));
        LOG64_NEW_LINE;
#endif
      }
    }

    // return signal strength in %
    inline uint8_t signal_strength()
    {
      if (!first)
      {
        return 0;
      }
      int16_t rssi_ = ((rssi - IoTGateway_MIN_RSSI) * 100) / (IoTGateway_MIN_RSSI * (-1));
      if (rssi_ < 0)
      {
        rssi_ = 0;
      }
      else if (rssi_ > 100)
      {
        rssi_ = 100;
      }
      return rssi_;
    }


    //return total acknowledged messages send count
    inline uint32_t ack_messages()
    {
      return total_ack;
    }

    //return received messages count for the gateway
    inline uint32_t recv_messages()
    {
      return total_recv;
    }

    //return all received messages count
    inline uint32_t all_messages()
    {
      return total_all;
    }

  public :

    // please call in the main loop to be able to dispatch data and menage logic
    inline void work()
    {

      if (q == NULL)
      {
        uint8_t buf_[IoTGateway_MAX_MESSAGE_LEN];
        uint8_t size_ = IoTGateway_MAX_MESSAGE_LEN;

        if (driver.recv(buf_, &size_))
        {
          total_all++;

          //#if defined (LOG64_ENABLED)
          //          LOG64_SET(F("IoTG: RECEIVED :"));
          //          for (uint8_t i = 0; i < size_; i++)
          //          {
          //            LOG64_SET(buf_[i]);
          //          }
          //          LOG64_NEW_LINE;
          //#endif
          if (size_ <=  IoTGateway_MAX_MESSAGE_LEN)
          {
            if (crc_chk(buf_, size_))
            {
              uint64_t buf_id_to = get_id_to(buf_, 2);

              // make sure we check also for 10(open gateway) if we are open gateway
              if ((buf_id_to == get_my_dest_id()) || (buf_id_to == gateway_id))
              {
                rssi = driver.lastRssi();

                //possible ack
                int8_t msg_seq = get_seq(buf_, 1);
                if (msg_seq < 0)
                {
                  // ack
                  ack_dispatch( msg_seq * (-1)) ;
                }
                else
                {
                  total_recv++;
                  get_ack(buf_, buf, buf_size);
                  q = buf;
                  received(buf_, size_, rssi);
                }
              }
            }
            else
            {
#if defined (LOG64_ENABLED)
              LOG64_SET(F("IoTG: CRC CHECK FAILED"));
              LOG64_NEW_LINE;
#endif
            }
          }
        }
      }
      if (q != NULL)
      {
        bool active = driver.isChannelActive();
        if (active)
        {
          if (active_start == 0)
          {
            // clear low priority dispatch if waiting for free channel
            active_start_dispatch = 0;

            //start the waiting
            active_start = millis();
            wait_timeout = random(IoTGateway_ACK_TIMEOUT, 2 * IoTGateway_ACK_TIMEOUT);
          }
          else
          {
            if (((uint32_t)(((uint32_t)millis()) - active_start)) >= wait_timeout);
            {
              active = false;
#if defined (LOG64_ENABLED)
              LOG64_SET("IoTG : ACTIVE TIMEOUT HIT");
              LOG64_NEW_LINE;
#endif
            }
          }
        }
        if (!active)
        {
          send_to_driver(buf, buf_size);
          total_ack++;
          active_start = 0;
          q = NULL;
        }
      }
      else  if (dispatch_buf_size != 0)
      {
        // no more then one every 500ms
        if (((uint32_t)(((uint32_t)millis()) - dispatch_last_execute)) > IoTGateway_DISPATCH_TIMEOUT)
        {
          // may be something for dispatch ?
          uint8_t local_buf[IoTGateway_MAX_MESSAGE_LEN];
          uint8_t local_buf_size;

          get_dispatch(local_buf, local_buf_size);

          if (local_buf_size > 0)
          {
            // we really have something for dispatch
            bool active = driver.isChannelActive();
            if (active)
            {
              if (active_start_dispatch == 0)
              {
                //start the waiting
                active_start_dispatch = millis();
                wait_timeout = random(IoTGateway_ACK_TIMEOUT, 2 * IoTGateway_ACK_TIMEOUT);
              }
              else
              {
                if (((uint32_t)(((uint32_t)millis()) - active_start_dispatch)) >= wait_timeout);
                {
                  active = false;
#if defined (LOG64_ENABLED)
                  LOG64_SET("IoTG : DISPATCH : ACTIVE TIMEOUT HIT");
                  LOG64_NEW_LINE;
#endif
                }
              }
            }
            if (!active)
            {
              send_to_driver(local_buf, local_buf_size);
              active_start_dispatch = 0;
              dispatch_last_execute = millis();
            }
          }
          else
          {
            // we do not have what to dispatch just skip to next timeout
            dispatch_last_execute = millis();
          }
        }
      }
    };

  private :

    inline uint64_t get_my_dest_id()
    {
      if (gateway_id & 1)
      {
        return gateway_id;
      }

      return 10LL;
    }

    // calculate 4 byte checksum
    inline uint32_t crc4(uint8_t buf_[],
                         uint8_t size_)
    {
      uint32_t res = 0;
      for (uint8_t i = 0; i < size_; i++)
      {
        uint8_t c = buf_[i];
        res = (res << 1) ^ c;
        res = res & 0xFFFFFFFF;
      }
      return res;
    };


    // calculate checksum
    inline uint8_t crc(uint8_t buf_[],
                       uint16_t end_index_,
                       uint16_t start_index)
    {
      uint32_t res = IoTGateway_MAGIC;  // add magic
      for (uint16_t i = start_index; i < end_index_; i++)
      {
        uint8_t c = buf_[i];
        res = (res << 1) ^ c;
        res = res & 0xFFFFFFFF;
      }
      return (uint8_t)(res & 0xFF);
    };

    // check message checksum
    inline  bool crc_chk(uint8_t buf_[],
                         uint8_t size_)
    {
      return (buf_[0] == crc(buf_, size_, 1));
    };

    // extract the sequence from a message
    inline int8_t get_seq(uint8_t buf_[], uint8_t pos)
    {
      return (int8_t)buf_[pos];
    };

    // extract the id to from a message
    inline uint64_t get_id_to(uint8_t buf_[], uint8_t pos_)
    {
      return get_ots(buf_, pos_);
    };

    // format byte(len), byte(crc), byte(seq), byte[4](receiver_id)
    // len 7
    // seq >=-127 - 0 is not valid sequence
    inline void get_ack(uint8_t buf_[], uint8_t buf_ack[], uint8_t & ack_size)
    {
      uint8_t pos_;
      // read
      pos_ = 2;
      uint64_t to_id = get_ots(buf_, pos_);
      uint64_t from_id = get_ots(buf_, pos_);
      // write
      //buf_ack[0] = crc;
      buf_ack[1] = (uint8_t)(((int8_t)buf_[1]) * ((int8_t) - 1));
      pos_ = 2;
      put_ots(from_id, buf_ack, pos_ );
      buf_ack[0] = crc(buf_ack, pos_ , 1);
      ack_size = pos_;
    }

    // send message to driver
    inline void send_to_driver(uint8_t buf_[], uint8_t size_)
    {
      if (!driver.send(buf_, size_))
      {
#if defined (LOG64_ENABLED)
        LOG64_SET(F("IoTG: SEND FAILED"));
        LOG64_NEW_LINE;
#endif
      }
      else if (!driver.waitPacketSent(IoTGateway_ACK_TIMEOUT))
      {
        // need reinit
#if defined (LOG64_ENABLED)
        LOG64_SET(F("IoTG: DRIVER BLOCKED : RESET"));
        LOG64_NEW_LINE;
#endif
        init(freq);

        return;
      }

      first = true;

    };
  private:
    inline uint64_t get_ots(uint8_t buf_[], uint8_t & pos_)
    {
      uint8_t start_ = pos_;
      switch (buf_[start_] & 0x3)
      {
        case 0:
          {
            pos_ += 1;
            return (uint64_t) ((buf_[start_] >> 2) & 0x3F);
          }
        case 1:
          {
            pos_ += 2;
            return (uint64_t)((((buf_[start_ + 1] << 8) | (buf_[start_])) >> 2) & 0x3FFF);
          }
        case 2:
          {
            pos_ += 4;
            return (uint64_t)((((((uint32_t)buf_[start_ + 3]) << 24) | (((uint32_t)buf_[start_ + 2]) << 16) | (((uint32_t)buf_[start_ + 1]) << 8) | ((uint32_t)buf_[start_])) >> 2) & 0x3FFFFFFFL);
          }
        case 3:
          {
            pos_ += 8;
            return  (((((uint64_t)buf_[start_ + 7]) << 56) | (((uint64_t)buf_[start_ + 6]) << 48) | (((uint64_t)buf_[start_ + 5]) << 40) | (((uint64_t)buf_[start_ + 4]) >> 32) | (((uint64_t)buf_[start_ + 3]) << 24)
                      | (((uint64_t)buf_[start_ + 2]) << 16) | (((uint64_t)buf_[start_ + 1]) << 8) | ((uint64_t)buf_[start_])) >> 2) & 0x3FFFFFFFFFFFFFFFLL;
          }
      }
    }

    inline void put_ots(uint64_t v, uint8_t buf_[], uint8_t & pos_ )
    {
      v = (v << 2);
      uint8_t start_ = pos_;
      if (v < 0x40LL)
      {
        buf_[start_] = (uint8_t) (v & 0xFFLL);
        pos_ += 1;
      }
      else if (v < 0x4000LL)
      {
        v = v | 1;
        buf_[start_ + 1] = (uint8_t)((v >> 8) & 0xFFLL);
        buf_[start_] = (uint8_t) (v & 0xFFLL);
        pos_ += 2;
      }
      else if (v < 0x40000000LL)
      {
        v = v | 2;
        buf_[start_ + 3] = (uint8_t)((v >> 24) & 0xFFLL);
        buf_[start_ + 2] = (uint8_t)((v >> 16) & 0xFFLL);
        buf_[start_ + 1] = (uint8_t)((v >> 8) & 0xFFLL);
        buf_[start_] = (uint8_t) (v & 0xFFLL);
        pos_ += 4;
      }
      else if (v < 0x4000000000000000LL)
      {
        v = v | 3;
        buf_[start_ + 7] = (uint8_t)((v >> 56) & 0xFFLL);
        buf_[start_ + 6] = (uint8_t)((v >> 48) & 0xFFLL);
        buf_[start_ + 5] = (uint8_t)((v >> 40) & 0xFFLL);
        buf_[start_ + 4] = (uint8_t)((v >> 32) & 0xFFLL);
        buf_[start_ + 3] = (uint8_t)((v >> 24) & 0xFFLL);
        buf_[start_ + 2] = (uint8_t)((v >> 16) & 0xFFLL);
        buf_[start_ + 1] = (uint8_t)((v >> 8) & 0xFFLL);
        buf_[start_] = (uint8_t) (v & 0xFFLL);
        pos_ += 8;
      }
    }
  private :
    uint8_t buf[IoTGateway_MAX_MESSAGE_LEN];
    uint8_t buf_size;

    uint8_t * q; // used as flag

  private :
    uint32_t total_ack;
    uint32_t total_recv;
    uint32_t total_all;

    bool first;

    uint8_t resetPin;
    void (* received)(uint8_t buf[], uint8_t size, int16_t rssi);

    RH_RF95_IoTGateway driver;
    int16_t rssi;

    uint32_t active_start;
    uint32_t active_start_dispatch;
    uint32_t wait_timeout;

    float freq;

    uint8_t gateway_key[16];
    uint64_t gateway_id;

    uint8_t dispatch_buf[IoTGateway_MAX_DISPATCH_MSG * (IoTGateway_MAX_MESSAGE_LEN + 1)];
    uint8_t dispatch_buf_size;
    uint16_t last_index_disatch_buf;
    uint32_t dispatch_last_execute;

  private :

    /**********************************************************\
      |                                                          |
      | XXTEA encryption algorithm library for C.                |
      |                                                          |
      | Encryption Algorithm Authors:                            |
      |      David J. Wheeler                                    |
      |      Roger M. Needham                                    |
      |                                                          |
      | Code Authors: Chen fei <cf850118@163.com>                |
      |               Ma Bingyao <mabingyao@gmail.com>           |
      | LastModified: Feb 7, 2016                                |
      |                                                          |
      | With changes                                             |
      |                                                          |
      | License (MIT)                                            |
      |                                                          |
      \**********************************************************/

#define MX (((z >> 5) ^ (y << 2)) + ((y >> 3) ^ (z << 4))) ^ ((sum ^ y) + (key[(p & 3) ^ e] ^ z))
#define DELTA 0x9e3779b9

    inline uint32_t * xxtea_to_uint_array_size(const uint8_t * data, size_t len, size_t * out_len) {
      uint32_t *out;

      ++len;
      size_t n = (((len & 3) == 0) ? (len >> 2) : ((len >> 2) + 1));
      if (n < 2) n = 2;
      --len;

      out = (uint32_t *)malloc(n << 2);
      if (!out) return NULL;
      *out_len = n;

      memcpy(&((uint8_t *)out)[1], data, len);
      ((uint8_t *)out)[0] = (uint8_t)len;

      return out;
    }


    inline uint32_t * xxtea_to_uint_array(const uint8_t * data, size_t len, size_t * out_len) {
      uint32_t *out;

      out = (uint32_t *)malloc(len);

      memcpy(out, data, len);

      *out_len = len >> 2;

      return out;
    }

    inline uint8_t * xxtea_to_ubyte_array_size(const uint32_t * data, size_t len, size_t * out_len) {
      uint8_t *out;
      size_t n;

      n = ((uint8_t *)data)[0];
      out = (uint8_t *)malloc(n);

      memcpy(out, &((uint8_t *)data)[1], n);

      *out_len = n;

      return out;
    }

    inline uint8_t * xxtea_to_ubyte_array(const uint32_t * data, size_t len, size_t * out_len) {
      uint8_t *out;
      size_t n;

      n = len << 2;
      out = (uint8_t *)malloc(n);

      memcpy(out, data, n);

      *out_len = n;

      return out;
    }

    inline uint32_t * xxtea_uint_decrypt(uint32_t * data, size_t len, uint32_t * key)
    {
      uint32_t n = (uint32_t)len - 1;
      uint32_t z, y = data[0], p, q = 6 + 52 / (n + 1), sum = q * DELTA, e;

      if (n < 1) return data;

      while (sum != 0)
      {
        e = sum >> 2 & 3;

        for (p = n; p > 0; p--)
        {
          z = data[p - 1];
          y = data[p] -= MX;
        }

        z = data[n];
        y = data[0] -= MX;
        sum -= DELTA;
      }

      return data;
    }

    inline uint32_t * xxtea_uint_encrypt(uint32_t * data, size_t len, uint32_t * key)
    {
      uint32_t n = (uint32_t)len - 1;
      uint32_t z = data[n], y, p, q = 6 + 52 / (n + 1), sum = 0, e;

      if (n < 1) return data;

      while (0 < q--)
      {
        sum += DELTA;
        e = sum >> 2 & 3;

        for (p = 0; p < n; p++)
        {
          y = data[p + 1];
          z = data[p] += MX;
        }

        y = data[0];
        z = data[n] += MX;
      }

      return data;
    }


    inline uint8_t * xxtea_encrypt(const uint8_t * data, size_t len, const uint8_t * key, size_t * out_len)
    {
      uint8_t *out;
      uint32_t *data_array;
      size_t data_len;

      if (!len) return NULL;

      data_array = xxtea_to_uint_array_size(data, len, &data_len);

      if (!data_array) return NULL;

      out = xxtea_to_ubyte_array(xxtea_uint_encrypt(data_array, data_len, (uint32_t *)key), data_len, out_len);

      free(data_array);

      return out;
    }


    inline uint8_t * xxtea_decrypt(const uint8_t * data, size_t len, const uint8_t * key, size_t * out_len)
    {
      uint8_t *out;
      uint32_t *data_array;
      size_t data_len;

      if (!len) return NULL;

      data_array = xxtea_to_uint_array(data, len, &data_len);
      if (!data_array) return NULL;


      out = xxtea_to_ubyte_array_size(xxtea_uint_decrypt(data_array, data_len, (uint32_t *)key), data_len, out_len);

      free(data_array);

      return out;
    }

};
























