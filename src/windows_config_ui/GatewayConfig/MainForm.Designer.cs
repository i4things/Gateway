namespace GatewayConfig
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.groupBoxWIFI = new System.Windows.Forms.GroupBox();
            this.buttonWIFISEND = new System.Windows.Forms.Button();
            this.labelPASS = new System.Windows.Forms.Label();
            this.textBoxPASS = new System.Windows.Forms.TextBox();
            this.labelSSID = new System.Windows.Forms.Label();
            this.textBoxSSID = new System.Windows.Forms.TextBox();
            this.groupBoxSITE = new System.Windows.Forms.GroupBox();
            this.buttonSITESEND = new System.Windows.Forms.Button();
            this.labelKEY = new System.Windows.Forms.Label();
            this.textBoxKEY = new System.Windows.Forms.TextBox();
            this.labelID = new System.Windows.Forms.Label();
            this.textBoxID = new System.Windows.Forms.TextBox();
            this.groupBoxMain = new System.Windows.Forms.GroupBox();
            this.buttonGet = new System.Windows.Forms.Button();
            this.buttonREFRESH = new System.Windows.Forms.Button();
            this.buttonCONNECT = new System.Windows.Forms.Button();
            this.labelPORT = new System.Windows.Forms.Label();
            this.comboBoxPORT = new System.Windows.Forms.ComboBox();
            this.groupBoxLOG = new System.Windows.Forms.GroupBox();
            this.buttonCopy = new System.Windows.Forms.Button();
            this.textBoxLOG = new System.Windows.Forms.TextBox();
            this.buttonLOGCLEAR = new System.Windows.Forms.Button();
            this.serialPort = new System.IO.Ports.SerialPort(this.components);
            this.timerSerial = new System.Windows.Forms.Timer(this.components);
            this.groupBoxFREQ = new System.Windows.Forms.GroupBox();
            this.buttonFREQ = new System.Windows.Forms.Button();
            this.labelFREQ = new System.Windows.Forms.Label();
            this.textBoxFREQ = new System.Windows.Forms.TextBox();
            this.groupBoxWIFI.SuspendLayout();
            this.groupBoxSITE.SuspendLayout();
            this.groupBoxMain.SuspendLayout();
            this.groupBoxLOG.SuspendLayout();
            this.groupBoxFREQ.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxWIFI
            // 
            this.groupBoxWIFI.Controls.Add(this.buttonWIFISEND);
            this.groupBoxWIFI.Controls.Add(this.labelPASS);
            this.groupBoxWIFI.Controls.Add(this.textBoxPASS);
            this.groupBoxWIFI.Controls.Add(this.labelSSID);
            this.groupBoxWIFI.Controls.Add(this.textBoxSSID);
            this.groupBoxWIFI.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBoxWIFI.Location = new System.Drawing.Point(0, 205);
            this.groupBoxWIFI.Margin = new System.Windows.Forms.Padding(4);
            this.groupBoxWIFI.Name = "groupBoxWIFI";
            this.groupBoxWIFI.Padding = new System.Windows.Forms.Padding(4);
            this.groupBoxWIFI.Size = new System.Drawing.Size(696, 178);
            this.groupBoxWIFI.TabIndex = 17;
            this.groupBoxWIFI.TabStop = false;
            this.groupBoxWIFI.Text = "WIFI";
            // 
            // buttonWIFISEND
            // 
            this.buttonWIFISEND.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonWIFISEND.Location = new System.Drawing.Point(462, 119);
            this.buttonWIFISEND.Margin = new System.Windows.Forms.Padding(4);
            this.buttonWIFISEND.Name = "buttonWIFISEND";
            this.buttonWIFISEND.Size = new System.Drawing.Size(215, 44);
            this.buttonWIFISEND.TabIndex = 9;
            this.buttonWIFISEND.Text = "Send to Gateway";
            this.buttonWIFISEND.UseVisualStyleBackColor = true;
            this.buttonWIFISEND.Click += new System.EventHandler(this.buttonWIFISEND_Click);
            // 
            // labelPASS
            // 
            this.labelPASS.AutoSize = true;
            this.labelPASS.Location = new System.Drawing.Point(15, 79);
            this.labelPASS.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelPASS.Name = "labelPASS";
            this.labelPASS.Size = new System.Drawing.Size(68, 25);
            this.labelPASS.TabIndex = 8;
            this.labelPASS.Text = "PASS";
            // 
            // textBoxPASS
            // 
            this.textBoxPASS.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxPASS.Location = new System.Drawing.Point(87, 76);
            this.textBoxPASS.Margin = new System.Windows.Forms.Padding(4);
            this.textBoxPASS.Name = "textBoxPASS";
            this.textBoxPASS.Size = new System.Drawing.Size(584, 31);
            this.textBoxPASS.TabIndex = 7;
            // 
            // labelSSID
            // 
            this.labelSSID.AutoSize = true;
            this.labelSSID.Location = new System.Drawing.Point(15, 38);
            this.labelSSID.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelSSID.Name = "labelSSID";
            this.labelSSID.Size = new System.Drawing.Size(60, 25);
            this.labelSSID.TabIndex = 6;
            this.labelSSID.Text = "SSID";
            // 
            // textBoxSSID
            // 
            this.textBoxSSID.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxSSID.Location = new System.Drawing.Point(87, 34);
            this.textBoxSSID.Margin = new System.Windows.Forms.Padding(4);
            this.textBoxSSID.Name = "textBoxSSID";
            this.textBoxSSID.Size = new System.Drawing.Size(584, 31);
            this.textBoxSSID.TabIndex = 5;
            // 
            // groupBoxSITE
            // 
            this.groupBoxSITE.Controls.Add(this.buttonSITESEND);
            this.groupBoxSITE.Controls.Add(this.labelKEY);
            this.groupBoxSITE.Controls.Add(this.textBoxKEY);
            this.groupBoxSITE.Controls.Add(this.labelID);
            this.groupBoxSITE.Controls.Add(this.textBoxID);
            this.groupBoxSITE.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBoxSITE.Location = new System.Drawing.Point(0, 383);
            this.groupBoxSITE.Margin = new System.Windows.Forms.Padding(4);
            this.groupBoxSITE.Name = "groupBoxSITE";
            this.groupBoxSITE.Padding = new System.Windows.Forms.Padding(4);
            this.groupBoxSITE.Size = new System.Drawing.Size(696, 199);
            this.groupBoxSITE.TabIndex = 18;
            this.groupBoxSITE.TabStop = false;
            this.groupBoxSITE.Text = "SITE";
            // 
            // buttonSITESEND
            // 
            this.buttonSITESEND.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonSITESEND.Location = new System.Drawing.Point(461, 136);
            this.buttonSITESEND.Margin = new System.Windows.Forms.Padding(4);
            this.buttonSITESEND.Name = "buttonSITESEND";
            this.buttonSITESEND.Size = new System.Drawing.Size(215, 44);
            this.buttonSITESEND.TabIndex = 21;
            this.buttonSITESEND.Text = "Send to Gateway";
            this.buttonSITESEND.UseVisualStyleBackColor = true;
            this.buttonSITESEND.Click += new System.EventHandler(this.buttonGATEWAYSEND_Click);
            // 
            // labelKEY
            // 
            this.labelKEY.AutoSize = true;
            this.labelKEY.Location = new System.Drawing.Point(19, 95);
            this.labelKEY.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelKEY.Name = "labelKEY";
            this.labelKEY.Size = new System.Drawing.Size(167, 25);
            this.labelKEY.TabIndex = 20;
            this.labelKEY.Text = "GATEWAY KEY";
            // 
            // textBoxKEY
            // 
            this.textBoxKEY.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxKEY.Location = new System.Drawing.Point(194, 92);
            this.textBoxKEY.Margin = new System.Windows.Forms.Padding(4);
            this.textBoxKEY.Name = "textBoxKEY";
            this.textBoxKEY.Size = new System.Drawing.Size(475, 31);
            this.textBoxKEY.TabIndex = 19;
            this.textBoxKEY.TextChanged += new System.EventHandler(this.GATEWAY_KEY_Changed);
            this.textBoxKEY.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.GATEWAY_KEY_KeyPress);
            // 
            // labelID
            // 
            this.labelID.AutoSize = true;
            this.labelID.Location = new System.Drawing.Point(19, 55);
            this.labelID.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelID.Name = "labelID";
            this.labelID.Size = new System.Drawing.Size(144, 25);
            this.labelID.TabIndex = 18;
            this.labelID.Text = "GATEWAY ID";
            // 
            // textBoxID
            // 
            this.textBoxID.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxID.Location = new System.Drawing.Point(194, 50);
            this.textBoxID.Margin = new System.Windows.Forms.Padding(4);
            this.textBoxID.Name = "textBoxID";
            this.textBoxID.Size = new System.Drawing.Size(475, 31);
            this.textBoxID.TabIndex = 17;
            this.textBoxID.TextChanged += new System.EventHandler(this.GATEWAY_ID_Changed);
            this.textBoxID.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.GATEWAY_ID_KeyPress);
            // 
            // groupBoxMain
            // 
            this.groupBoxMain.Controls.Add(this.buttonGet);
            this.groupBoxMain.Controls.Add(this.buttonREFRESH);
            this.groupBoxMain.Controls.Add(this.buttonCONNECT);
            this.groupBoxMain.Controls.Add(this.labelPORT);
            this.groupBoxMain.Controls.Add(this.comboBoxPORT);
            this.groupBoxMain.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBoxMain.Location = new System.Drawing.Point(0, 0);
            this.groupBoxMain.Margin = new System.Windows.Forms.Padding(4);
            this.groupBoxMain.Name = "groupBoxMain";
            this.groupBoxMain.Padding = new System.Windows.Forms.Padding(4);
            this.groupBoxMain.Size = new System.Drawing.Size(696, 205);
            this.groupBoxMain.TabIndex = 19;
            this.groupBoxMain.TabStop = false;
            this.groupBoxMain.Text = "MAIN";
            // 
            // buttonGet
            // 
            this.buttonGet.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonGet.Enabled = false;
            this.buttonGet.Location = new System.Drawing.Point(474, 141);
            this.buttonGet.Margin = new System.Windows.Forms.Padding(4);
            this.buttonGet.Name = "buttonGet";
            this.buttonGet.Size = new System.Drawing.Size(213, 44);
            this.buttonGet.TabIndex = 15;
            this.buttonGet.Text = "Get Configuration";
            this.buttonGet.UseVisualStyleBackColor = true;
            this.buttonGet.Click += new System.EventHandler(this.buttonGet_Click);
            // 
            // buttonREFRESH
            // 
            this.buttonREFRESH.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonREFRESH.Location = new System.Drawing.Point(474, 29);
            this.buttonREFRESH.Margin = new System.Windows.Forms.Padding(4);
            this.buttonREFRESH.Name = "buttonREFRESH";
            this.buttonREFRESH.Size = new System.Drawing.Size(213, 44);
            this.buttonREFRESH.TabIndex = 14;
            this.buttonREFRESH.Text = "Refresh";
            this.buttonREFRESH.UseVisualStyleBackColor = true;
            this.buttonREFRESH.Click += new System.EventHandler(this.buttonREFRESH_Click);
            // 
            // buttonCONNECT
            // 
            this.buttonCONNECT.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonCONNECT.Location = new System.Drawing.Point(474, 86);
            this.buttonCONNECT.Margin = new System.Windows.Forms.Padding(4);
            this.buttonCONNECT.Name = "buttonCONNECT";
            this.buttonCONNECT.Size = new System.Drawing.Size(213, 44);
            this.buttonCONNECT.TabIndex = 13;
            this.buttonCONNECT.Text = "Connect";
            this.buttonCONNECT.UseVisualStyleBackColor = true;
            this.buttonCONNECT.Click += new System.EventHandler(this.buttonCONNECT_Click);
            // 
            // labelPORT
            // 
            this.labelPORT.AutoSize = true;
            this.labelPORT.Location = new System.Drawing.Point(15, 36);
            this.labelPORT.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelPORT.Name = "labelPORT";
            this.labelPORT.Size = new System.Drawing.Size(70, 25);
            this.labelPORT.TabIndex = 12;
            this.labelPORT.Text = "PORT";
            // 
            // comboBoxPORT
            // 
            this.comboBoxPORT.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxPORT.FormattingEnabled = true;
            this.comboBoxPORT.Location = new System.Drawing.Point(112, 31);
            this.comboBoxPORT.Margin = new System.Windows.Forms.Padding(4);
            this.comboBoxPORT.Name = "comboBoxPORT";
            this.comboBoxPORT.Size = new System.Drawing.Size(336, 33);
            this.comboBoxPORT.TabIndex = 11;
            // 
            // groupBoxLOG
            // 
            this.groupBoxLOG.Controls.Add(this.buttonCopy);
            this.groupBoxLOG.Controls.Add(this.textBoxLOG);
            this.groupBoxLOG.Controls.Add(this.buttonLOGCLEAR);
            this.groupBoxLOG.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBoxLOG.Location = new System.Drawing.Point(0, 720);
            this.groupBoxLOG.Margin = new System.Windows.Forms.Padding(4);
            this.groupBoxLOG.Name = "groupBoxLOG";
            this.groupBoxLOG.Padding = new System.Windows.Forms.Padding(4);
            this.groupBoxLOG.Size = new System.Drawing.Size(696, 309);
            this.groupBoxLOG.TabIndex = 20;
            this.groupBoxLOG.TabStop = false;
            this.groupBoxLOG.Text = "LOG";
            // 
            // buttonCopy
            // 
            this.buttonCopy.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.buttonCopy.Location = new System.Drawing.Point(24, 246);
            this.buttonCopy.Margin = new System.Windows.Forms.Padding(4);
            this.buttonCopy.Name = "buttonCopy";
            this.buttonCopy.Size = new System.Drawing.Size(215, 44);
            this.buttonCopy.TabIndex = 24;
            this.buttonCopy.Text = "Copy To Clipboard";
            this.buttonCopy.UseVisualStyleBackColor = true;
            this.buttonCopy.Click += new System.EventHandler(this.buttonCopy_Click);
            // 
            // textBoxLOG
            // 
            this.textBoxLOG.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxLOG.Location = new System.Drawing.Point(26, 44);
            this.textBoxLOG.Margin = new System.Windows.Forms.Padding(4);
            this.textBoxLOG.Multiline = true;
            this.textBoxLOG.Name = "textBoxLOG";
            this.textBoxLOG.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxLOG.Size = new System.Drawing.Size(643, 174);
            this.textBoxLOG.TabIndex = 23;
            // 
            // buttonLOGCLEAR
            // 
            this.buttonLOGCLEAR.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonLOGCLEAR.Location = new System.Drawing.Point(456, 246);
            this.buttonLOGCLEAR.Margin = new System.Windows.Forms.Padding(4);
            this.buttonLOGCLEAR.Name = "buttonLOGCLEAR";
            this.buttonLOGCLEAR.Size = new System.Drawing.Size(215, 44);
            this.buttonLOGCLEAR.TabIndex = 22;
            this.buttonLOGCLEAR.Text = "Clear";
            this.buttonLOGCLEAR.UseVisualStyleBackColor = true;
            this.buttonLOGCLEAR.Click += new System.EventHandler(this.buttonLOGCLEAR_Click);
            // 
            // serialPort
            // 
            this.serialPort.BaudRate = 115200;
            this.serialPort.ErrorReceived += new System.IO.Ports.SerialErrorReceivedEventHandler(this.serialPort_ErrorReceived);
            this.serialPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort_DataReceived);
            // 
            // timerSerial
            // 
            this.timerSerial.Interval = 1000;
            this.timerSerial.Tick += new System.EventHandler(this.timerSerial_Tick);
            // 
            // groupBoxFREQ
            // 
            this.groupBoxFREQ.Controls.Add(this.buttonFREQ);
            this.groupBoxFREQ.Controls.Add(this.labelFREQ);
            this.groupBoxFREQ.Controls.Add(this.textBoxFREQ);
            this.groupBoxFREQ.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBoxFREQ.Location = new System.Drawing.Point(0, 582);
            this.groupBoxFREQ.Margin = new System.Windows.Forms.Padding(4);
            this.groupBoxFREQ.Name = "groupBoxFREQ";
            this.groupBoxFREQ.Padding = new System.Windows.Forms.Padding(4);
            this.groupBoxFREQ.Size = new System.Drawing.Size(696, 138);
            this.groupBoxFREQ.TabIndex = 21;
            this.groupBoxFREQ.TabStop = false;
            this.groupBoxFREQ.Text = "FREQUENCY";
            // 
            // buttonFREQ
            // 
            this.buttonFREQ.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonFREQ.Location = new System.Drawing.Point(461, 92);
            this.buttonFREQ.Margin = new System.Windows.Forms.Padding(4);
            this.buttonFREQ.Name = "buttonFREQ";
            this.buttonFREQ.Size = new System.Drawing.Size(215, 44);
            this.buttonFREQ.TabIndex = 21;
            this.buttonFREQ.Text = "Send to Gateway";
            this.buttonFREQ.UseVisualStyleBackColor = true;
            this.buttonFREQ.Click += new System.EventHandler(this.buttonFREQ_Click);
            // 
            // labelFREQ
            // 
            this.labelFREQ.AutoSize = true;
            this.labelFREQ.Location = new System.Drawing.Point(19, 51);
            this.labelFREQ.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelFREQ.Name = "labelFREQ";
            this.labelFREQ.Size = new System.Drawing.Size(144, 25);
            this.labelFREQ.TabIndex = 20;
            this.labelFREQ.Text = "FREQUENCY";
            // 
            // textBoxFREQ
            // 
            this.textBoxFREQ.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxFREQ.Location = new System.Drawing.Point(184, 48);
            this.textBoxFREQ.Margin = new System.Windows.Forms.Padding(4);
            this.textBoxFREQ.Name = "textBoxFREQ";
            this.textBoxFREQ.Size = new System.Drawing.Size(485, 31);
            this.textBoxFREQ.TabIndex = 19;
            this.textBoxFREQ.TextChanged += new System.EventHandler(this.textBoxFREQ_TextChanged);
            this.textBoxFREQ.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.textBoxFREQ_KeyPress);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(696, 1029);
            this.Controls.Add(this.groupBoxLOG);
            this.Controls.Add(this.groupBoxFREQ);
            this.Controls.Add(this.groupBoxSITE);
            this.Controls.Add(this.groupBoxWIFI);
            this.Controls.Add(this.groupBoxMain);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MinimumSize = new System.Drawing.Size(722, 1100);
            this.Name = "MainForm";
            this.Text = "Gateway Configurator";
            this.Load += new System.EventHandler(this.MainFomr_Load);
            this.groupBoxWIFI.ResumeLayout(false);
            this.groupBoxWIFI.PerformLayout();
            this.groupBoxSITE.ResumeLayout(false);
            this.groupBoxSITE.PerformLayout();
            this.groupBoxMain.ResumeLayout(false);
            this.groupBoxMain.PerformLayout();
            this.groupBoxLOG.ResumeLayout(false);
            this.groupBoxLOG.PerformLayout();
            this.groupBoxFREQ.ResumeLayout(false);
            this.groupBoxFREQ.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxWIFI;
        private System.Windows.Forms.Button buttonWIFISEND;
        private System.Windows.Forms.Label labelPASS;
        private System.Windows.Forms.TextBox textBoxPASS;
        private System.Windows.Forms.Label labelSSID;
        private System.Windows.Forms.TextBox textBoxSSID;
        private System.Windows.Forms.GroupBox groupBoxSITE;
        private System.Windows.Forms.Button buttonSITESEND;
        private System.Windows.Forms.Label labelKEY;
        private System.Windows.Forms.TextBox textBoxKEY;
        private System.Windows.Forms.Label labelID;
        private System.Windows.Forms.TextBox textBoxID;
        private System.Windows.Forms.GroupBox groupBoxMain;
        private System.Windows.Forms.Button buttonCONNECT;
        private System.Windows.Forms.Label labelPORT;
        private System.Windows.Forms.ComboBox comboBoxPORT;
        private System.Windows.Forms.GroupBox groupBoxLOG;
        private System.IO.Ports.SerialPort serialPort;
        private System.Windows.Forms.Button buttonREFRESH;
        private System.Windows.Forms.Button buttonLOGCLEAR;
        private System.Windows.Forms.Timer timerSerial;
        private System.Windows.Forms.TextBox textBoxLOG;
        private System.Windows.Forms.Button buttonCopy;
        private System.Windows.Forms.Button buttonGet;
        private System.Windows.Forms.GroupBox groupBoxFREQ;
        private System.Windows.Forms.Button buttonFREQ;
        private System.Windows.Forms.Label labelFREQ;
        private System.Windows.Forms.TextBox textBoxFREQ;
    }
}

