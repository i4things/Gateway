/* Copyright (c) B2N Ltd. All Rights Reserved.
 *
 * This SOURCE CODE FILE, which has been provided by B2N Ltd. as part of an B2N
 * Ltd. product for use ONLY by licensed users of the product, includes
 * Ltd. product for use ONLY by licensed users of the product, includes
 * CONFIDENTIAL and PROPRIETARY information of B2N Ltd.
 *
 * USE OF THIS SOFTWARE IS GOVERNED BY THE TERMS AND CONDITIONS OF THE LICENSE
 * STATEMENT AND LIMITED WARRANTY FURNISHED WITH THE PRODUCT.
 *
 * IN PARTICULAR, YOU WILL INDEMNIFY AND HOLD B2N LTD., ITS RELATED COMPANIES
 * AND ITS SUPPLIERS, HARMLESS FROM AND AGAINST ANY CLAIMS OR LIABILITIES
 * ARISING OUT OF THE USE, REPRODUCTION, OR DISTRIBUTION OF YOUR PROGRAMS,
 * INCLUDING ANY CLAIMS OR LIABILITIES ARISING OUT OF OR RESULTING FROM THE USE,
 * MODIFICATION, OR DISTRIBUTION OF PROGRAMS OR FILES CREATED FROM, BASED ON,
 * AND/OR DERIVED FROM THIS SOURCE CODE FILE.
 */

using System;
using System.Windows.Forms;
using System.Text;
using System.Collections.Generic;
using System.Threading;
using System.Globalization;

namespace GatewayConfig
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private bool updating = false;
        private string buffer = "";

        private static readonly int magicSIZE = 36;
        private static readonly string magicGET = "21041043-7AE3-47E0-85B7-9287181052BC";
        private static readonly string magicWIFI = "C4E4D104-5F0F-457F-BA94-88B020531276";
        private static readonly string magicGATEWAY = "03AD745F-AFA4-41A4-ADC7-119A111A1B47";
        private static readonly string magicFREQ = "2009B290-57A9-4ADE-BCA6-902EAC723674";

        private void GATEWAY_ID_Changed(object sender, EventArgs e)
        {
            if (updating)
            {
                return;
            }

            updating = true;

            string text = textBoxID.Text;

            int selectionStart = textBoxID.SelectionStart;
            int selectionLength = textBoxID.SelectionLength;

            textBoxID.Text = "";
            foreach (char c in text)
            {
                if (char.IsDigit(c))
                {
                    textBoxID.Text += c;
                }
            }

            textBoxID.SelectionStart = selectionStart;
            textBoxID.SelectionLength = selectionLength;

            updating = false;
        }

        private void GATEWAY_ID_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((!char.IsControl(e.KeyChar)) && (!char.IsDigit(e.KeyChar)))
            {
                e.Handled = true;
            }
            else
            {
                e.Handled = false;
            }
        }

        private void GATEWAY_KEY_Changed(object sender, EventArgs e)
        {
            if (updating)
            {
                return;
            }

            updating = true;

            string text = textBoxKEY.Text;

            int selectionStart = textBoxKEY.SelectionStart;
            int selectionLength = textBoxKEY.SelectionLength;

            textBoxKEY.Text = "";
            foreach (char c in text)
            {
                if (char.IsDigit(c) || (('A' <= c) && ('F' >= c)) || (('a' <= c) && ('f' >= c)))
                {
                    textBoxKEY.Text += c;
                }
            }

            textBoxKEY.SelectionStart = selectionStart;
            textBoxKEY.SelectionLength = selectionLength;

            updating = false;
        }

        private void GATEWAY_KEY_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((!char.IsControl(e.KeyChar)) && (!char.IsDigit(e.KeyChar)) && ('A' > e.KeyChar) && ('F' < e.KeyChar) &&  ('a' > e.KeyChar) && ('f' < e.KeyChar))
            {
                e.Handled = true;
            }
            else
            {
                e.Handled = false;
            }
        }

        private void comboBoxPORTLoad()
        {
            comboBoxPORT.Items.Clear();
            comboBoxPORT.SelectedIndex = -1;
            comboBoxPORT.Text = "";

            Dictionary<string, string> coms = new Dictionary<string, string>();

            foreach (string s in System.IO.Ports.SerialPort.GetPortNames())
            {
                string st = s.Trim();
                if (!coms.ContainsKey(st))
                {
                    coms.Add(st, st);
                    comboBoxPORT.Items.Add(st);
                }
            }

            if (comboBoxPORT.Items.Count > 0)
            {
                comboBoxPORT.SelectedIndex = 0;
            }
        }

        private void MainFomr_Load(object sender, EventArgs e)
        {
            comboBoxPORTLoad();
        }

        private void buttonREFRESH_Click(object sender, EventArgs e)
        {
            comboBoxPORTLoad();
        }

        private void buttonLOGCLEAR_Click(object sender, EventArgs e)
        {
            textBoxLOG.Text = "";
        }

        private void buttonCONNECT_Click(object sender, EventArgs e)
        {
            if (buttonCONNECT.Text.Equals("Connect"))
            {
                if (comboBoxPORT.SelectedIndex >= 0)
                {
                    try
                    {
                        

                        serialPort.PortName = comboBoxPORT.Text;
                        serialPort.Open();
                        timerSerial.Start();

                        buttonCONNECT.Text = "Disconnect";
                        buttonREFRESH.Enabled = false;
                        comboBoxPORT.Enabled = false;
                        buttonGet.Enabled = true;
                        
                    }
                    catch (Exception)
                    {
                        serialPortClose();
                        buttonCONNECT.Text = "Connect";
                        MessageBox.Show("Can't connect.(BUSY)", "Error");
                    }


                }
                else
                {
                    MessageBox.Show("No selected COM port.", "Error");
                }
            }
            else
            {
                serialPortClose();
                buttonCONNECT.Text = "Connect";
            }
        }

        private void serialPortClose()
        {
            try { timerSerial.Stop(); }
            catch (Exception) { };
            try { serialPort.Close(); }
            catch (Exception) { };
            try { serialPort.Dispose(); }
            catch (Exception) { };

            serialPort = new System.IO.Ports.SerialPort(this.components);
            serialPort.BaudRate = 115200;
            serialPort.ErrorReceived += new System.IO.Ports.SerialErrorReceivedEventHandler(this.serialPort_ErrorReceived);
            serialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort_DataReceived);

            buttonREFRESH.Enabled = true;
            comboBoxPORT.Enabled = true;
            buttonGet.Enabled = false;
        }

        private void serialPort_ErrorReceived(object sender, System.IO.Ports.SerialErrorReceivedEventArgs e)
        {
            //textBoxLOG.BeginInvoke((MethodInvoker)delegate
            //{

            //    serialPortClose();

            //    buttonCONNECT.Text = "Connect";

            //    MessageBox.Show("COM communication error", "Error");
            //});
        }

        private void serialPort_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            string s = serialPort.ReadExisting();
            textBoxLOG.BeginInvoke((MethodInvoker)delegate
            {
                foreach (char c in s)
                {
                    if ((c == 0) || (c == 13) || (c == 10))
                    {
                        // make sure we display only the actual log and not the communication
                        string data = Encoding.UTF8.GetString(CONFIG_BUF, 0, CONFIG_BUF_SIZE);
                        Console.Write(data + Environment.NewLine);
                        if (!PARSE(data))
                        {
                            if (!string.IsNullOrEmpty(data))
                            {
                                buffer += data;
                                buffer += Environment.NewLine;
                                buffer += DateTime.Now.ToString("HH:mm:ss");
                                buffer += ": ";
                            }
                        }
                    }
                    else
                    {
                        CONFIG_BUF[CONFIG_BUF_SIZE++] = (byte)c;
                        if (CONFIG_BUF_SIZE == CONFIG_BUF_MAX_SIZE)
                        {
                            CONFIG_BUF_SIZE = 0;
                        }
                    }
                }
            });
        }

        private void timerSerial_Tick(object sender, EventArgs e)
        {
            textBoxLOG.BeginInvoke((MethodInvoker)delegate
            {
                //buffer = Convert.ToString(new Random().Next(0,100)) + Environment.NewLine;
                if (!String.IsNullOrEmpty(buffer))
                {
                    textBoxLOG.AppendText(buffer);

                    buffer = "";
                }
            });
        }

        private void buttonCopy_Click(object sender, EventArgs e)
        {
            Clipboard.SetText(textBoxLOG.Text);
        }

        private static readonly byte CONFIG_BUF_MAX_SIZE = 200;
        byte[] CONFIG_BUF = new byte[CONFIG_BUF_MAX_SIZE];
        byte CONFIG_BUF_SIZE = 0;

        private const byte GATEWAY_ID_CODE = 1;
        private const byte GATEWAY_KEY_CODE = 2;
        private const byte SSID_CODE = 3;
        private const byte PASS_CODE = 4;
        private const byte FREQ_CODE = 5;
        private byte NEXT = 0;

        private static unsafe uint SingleToUInt32Bits(float value)
        {
            return *(uint*)(&value);
        }
        private static unsafe float UInt32BitsToSingle(uint value)
        {
            return *(float*)(&value);
        }


        private bool PARSE(string data)
        {
            bool ret = false;
            if (CONFIG_BUF_SIZE > 0)
            {
                if ((CONFIG_BUF_SIZE == magicSIZE) && (data.Equals(magicGATEWAY)))
                {
                    NEXT = GATEWAY_ID_CODE;
                    ret = true;
                }
                else if ((CONFIG_BUF_SIZE == magicSIZE) && (data.Equals(magicWIFI)))
                {
                    NEXT = SSID_CODE;
                    ret = true;
                }
                else if ((CONFIG_BUF_SIZE == magicSIZE) && (data.Equals(magicFREQ)))
                {
                    NEXT = FREQ_CODE;
                    ret = true;
                }
                else
                {
                    switch (NEXT)
                    {
                        case GATEWAY_ID_CODE: textBoxID.Text = data; NEXT = GATEWAY_KEY_CODE; ret = true; break;
                        case GATEWAY_KEY_CODE: textBoxKEY.Text = data; NEXT = 0; ret = true; break;
                        case SSID_CODE: textBoxSSID.Text = data; NEXT = PASS_CODE; ret = true; break;
                        case PASS_CODE: textBoxPASS.Text = data; NEXT = 0; ret = true; break;
                        case FREQ_CODE:
                            {
                                uint ui;
                                if (uint.TryParse(data, NumberStyles.Any, CultureInfo.InvariantCulture, out ui)) 
                                {
                                    textBoxFREQ.Text = UInt32BitsToSingle(ui).ToString(CultureInfo.CurrentCulture);
                                }
                                NEXT = 0; 
                                ret = true;
                            };
                            break;
                        default: NEXT = 0; break;
                    }
                }
            }
            CONFIG_BUF_SIZE = 0;
            return ret;
        }

        private void buttonWIFISEND_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.WriteLine(magicWIFI);
                serialPort.WriteLine(textBoxSSID.Text);
                serialPort.WriteLine(textBoxPASS.Text);
            }
            else
            {
                MessageBox.Show("Not connected.", "Error");
            }
        }

        private void buttonGATEWAYSEND_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.WriteLine(magicGATEWAY);
                serialPort.WriteLine(textBoxID.Text);
                serialPort.WriteLine(textBoxKEY.Text);
            }
            else
            {
                MessageBox.Show("Not connected.", "Error");
            }
        }

        private void buttonGet_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                serialPort.WriteLine(magicGET);
            }
            else
            {
                MessageBox.Show("Not connected.", "Error");
            }
        }


        private void textBoxFREQ_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((!char.IsControl(e.KeyChar)) && (!char.IsDigit(e.KeyChar)) && (e.KeyChar != CultureInfo.CurrentCulture.NumberFormat.NumberDecimalSeparator[0]))
            {
                e.Handled = true;
            }
            else
            {
                e.Handled = false;
            }
        }

        private void textBoxFREQ_TextChanged(object sender, EventArgs e)
        {
            if (updating)
            {
                return;
            }

            updating = true;

            string text = textBoxFREQ.Text;

            int selectionStart = textBoxFREQ.SelectionStart;
            int selectionLength = textBoxFREQ.SelectionLength;

            textBoxFREQ.Text = "";
            foreach (char c in text)
            {
                if (char.IsDigit(c) || ((c == CultureInfo.CurrentCulture.NumberFormat.NumberDecimalSeparator[0]) && (!textBoxFREQ.Text.Contains(CultureInfo.CurrentCulture.NumberFormat.NumberDecimalSeparator))))
                {
                    textBoxFREQ.Text += c;
                }
            }

            textBoxFREQ.SelectionStart = selectionStart;
            textBoxFREQ.SelectionLength = selectionLength;

            updating = false;
        }

        private void buttonFREQ_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                float f;
                if (float.TryParse(textBoxFREQ.Text, NumberStyles.Any, CultureInfo.InvariantCulture, out f))
                {
                    serialPort.WriteLine(magicFREQ);
                    serialPort.WriteLine(SingleToUInt32Bits(f).ToString());
                }
                else
                {
                    MessageBox.Show("Wrong number format.", "Error");
                }
            }
            else
            {
                MessageBox.Show("Not connected.", "Error");
            }
        }


        

    }
}
