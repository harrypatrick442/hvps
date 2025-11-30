namespace ProgrammingHelper
{
    partial class ProjectOptions
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            LabelProjectName = new Label();
            tableLayoutPanel1 = new TableLayoutPanel();
            tableLayoutPanel2 = new TableLayoutPanel();
            buttonBringToFront = new Button();
            buttonCtlBracket = new Button();
            _ButtonMonitor = new Button();
            _ButtonFlash = new Button();
            _ButtonSetup = new Button();
            _ButtonBuild = new Button();
            buttonUpdateComPort = new Button();
            tableLayoutPanel3 = new TableLayoutPanel();
            ButtonAll = new Button();
            tableLayoutPanel1.SuspendLayout();
            tableLayoutPanel2.SuspendLayout();
            tableLayoutPanel3.SuspendLayout();
            SuspendLayout();
            // 
            // LabelProjectName
            // 
            LabelProjectName.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            LabelProjectName.AutoSize = true;
            LabelProjectName.Location = new Point(3, 3);
            LabelProjectName.Margin = new Padding(3);
            LabelProjectName.Name = "LabelProjectName";
            LabelProjectName.Size = new Size(96, 11);
            LabelProjectName.TabIndex = 2;
            LabelProjectName.Text = "[Project Name]";
            // 
            // tableLayoutPanel1
            // 
            tableLayoutPanel1.ColumnCount = 2;
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 40F));
            tableLayoutPanel1.Controls.Add(tableLayoutPanel2, 0, 0);
            tableLayoutPanel1.Controls.Add(tableLayoutPanel3, 1, 0);
            tableLayoutPanel1.Dock = DockStyle.Fill;
            tableLayoutPanel1.Location = new Point(0, 0);
            tableLayoutPanel1.Margin = new Padding(0);
            tableLayoutPanel1.Name = "tableLayoutPanel1";
            tableLayoutPanel1.RowCount = 1;
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 100F));
            tableLayoutPanel1.Size = new Size(148, 148);
            tableLayoutPanel1.TabIndex = 3;
            // 
            // tableLayoutPanel2
            // 
            tableLayoutPanel2.ColumnCount = 1;
            tableLayoutPanel2.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
            tableLayoutPanel2.Controls.Add(buttonBringToFront, 0, 7);
            tableLayoutPanel2.Controls.Add(buttonCtlBracket, 0, 6);
            tableLayoutPanel2.Controls.Add(_ButtonMonitor, 0, 5);
            tableLayoutPanel2.Controls.Add(LabelProjectName, 0, 0);
            tableLayoutPanel2.Controls.Add(_ButtonFlash, 0, 4);
            tableLayoutPanel2.Controls.Add(_ButtonSetup, 0, 2);
            tableLayoutPanel2.Controls.Add(_ButtonBuild, 0, 3);
            tableLayoutPanel2.Controls.Add(buttonUpdateComPort, 0, 1);
            tableLayoutPanel2.Dock = DockStyle.Fill;
            tableLayoutPanel2.Location = new Point(3, 3);
            tableLayoutPanel2.Name = "tableLayoutPanel2";
            tableLayoutPanel2.RowCount = 8;
            tableLayoutPanel2.RowStyles.Add(new RowStyle(SizeType.Percent, 12.49895F));
            tableLayoutPanel2.RowStyles.Add(new RowStyle(SizeType.Percent, 12.5016737F));
            tableLayoutPanel2.RowStyles.Add(new RowStyle(SizeType.Percent, 12.4989538F));
            tableLayoutPanel2.RowStyles.Add(new RowStyle(SizeType.Percent, 12.4989529F));
            tableLayoutPanel2.RowStyles.Add(new RowStyle(SizeType.Percent, 12.4989538F));
            tableLayoutPanel2.RowStyles.Add(new RowStyle(SizeType.Percent, 12.4989538F));
            tableLayoutPanel2.RowStyles.Add(new RowStyle(SizeType.Percent, 12.50028F));
            tableLayoutPanel2.RowStyles.Add(new RowStyle(SizeType.Percent, 12.5032816F));
            tableLayoutPanel2.Size = new Size(102, 142);
            tableLayoutPanel2.TabIndex = 2;
            // 
            // buttonBringToFront
            // 
            buttonBringToFront.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            buttonBringToFront.BackColor = Color.FromArgb(192, 255, 255);
            buttonBringToFront.Dock = DockStyle.Fill;
            buttonBringToFront.Location = new Point(0, 119);
            buttonBringToFront.Margin = new Padding(0);
            buttonBringToFront.Name = "buttonBringToFront";
            buttonBringToFront.Size = new Size(102, 23);
            buttonBringToFront.TabIndex = 7;
            buttonBringToFront.Text = "Bring To Front";
            buttonBringToFront.UseVisualStyleBackColor = false;
            buttonBringToFront.Click += buttonBringToFront_Click;
            // 
            // buttonCtlBracket
            // 
            buttonCtlBracket.BackColor = Color.FromArgb(128, 64, 0);
            buttonCtlBracket.Dock = DockStyle.Fill;
            buttonCtlBracket.Location = new Point(0, 102);
            buttonCtlBracket.Margin = new Padding(0);
            buttonCtlBracket.Name = "buttonCtlBracket";
            buttonCtlBracket.Size = new Size(102, 17);
            buttonCtlBracket.TabIndex = 5;
            buttonCtlBracket.Text = "ctl + ]";
            buttonCtlBracket.UseVisualStyleBackColor = false;
            buttonCtlBracket.Click += buttonCtlBracket_Click;
            // 
            // _ButtonMonitor
            // 
            _ButtonMonitor.BackColor = Color.Fuchsia;
            _ButtonMonitor.Dock = DockStyle.Fill;
            _ButtonMonitor.Location = new Point(0, 85);
            _ButtonMonitor.Margin = new Padding(0);
            _ButtonMonitor.Name = "_ButtonMonitor";
            _ButtonMonitor.Size = new Size(102, 17);
            _ButtonMonitor.TabIndex = 3;
            _ButtonMonitor.Text = "Monitor";
            _ButtonMonitor.UseVisualStyleBackColor = false;
            _ButtonMonitor.Click += _ButtonMonitor_Click;
            // 
            // _ButtonFlash
            // 
            _ButtonFlash.BackColor = Color.FromArgb(255, 192, 128);
            _ButtonFlash.Dock = DockStyle.Fill;
            _ButtonFlash.Location = new Point(0, 68);
            _ButtonFlash.Margin = new Padding(0);
            _ButtonFlash.Name = "_ButtonFlash";
            _ButtonFlash.Size = new Size(102, 17);
            _ButtonFlash.TabIndex = 2;
            _ButtonFlash.Text = "Flash";
            _ButtonFlash.UseVisualStyleBackColor = false;
            _ButtonFlash.Click += _ButtonFlash_Click;
            // 
            // _ButtonSetup
            // 
            _ButtonSetup.BackColor = Color.FromArgb(255, 255, 128);
            _ButtonSetup.Dock = DockStyle.Fill;
            _ButtonSetup.Location = new Point(0, 34);
            _ButtonSetup.Margin = new Padding(0);
            _ButtonSetup.Name = "_ButtonSetup";
            _ButtonSetup.Size = new Size(102, 17);
            _ButtonSetup.TabIndex = 0;
            _ButtonSetup.Text = "Setup";
            _ButtonSetup.UseVisualStyleBackColor = false;
            _ButtonSetup.Click += _ButtonSetup_Click;
            // 
            // _ButtonBuild
            // 
            _ButtonBuild.BackColor = Color.Black;
            _ButtonBuild.Dock = DockStyle.Fill;
            _ButtonBuild.ForeColor = Color.White;
            _ButtonBuild.Location = new Point(0, 51);
            _ButtonBuild.Margin = new Padding(0);
            _ButtonBuild.Name = "_ButtonBuild";
            _ButtonBuild.Size = new Size(102, 17);
            _ButtonBuild.TabIndex = 1;
            _ButtonBuild.Text = "Build";
            _ButtonBuild.UseVisualStyleBackColor = false;
            _ButtonBuild.Click += _ButtonBuild_Click;
            // 
            // buttonUpdateComPort
            // 
            buttonUpdateComPort.Dock = DockStyle.Fill;
            buttonUpdateComPort.Location = new Point(0, 17);
            buttonUpdateComPort.Margin = new Padding(0);
            buttonUpdateComPort.Name = "buttonUpdateComPort";
            buttonUpdateComPort.Size = new Size(102, 17);
            buttonUpdateComPort.TabIndex = 6;
            buttonUpdateComPort.Text = "Update COM";
            buttonUpdateComPort.UseVisualStyleBackColor = true;
            buttonUpdateComPort.Click += buttonUpdateComPort_Click;
            // 
            // tableLayoutPanel3
            // 
            tableLayoutPanel3.ColumnCount = 1;
            tableLayoutPanel3.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
            tableLayoutPanel3.Controls.Add(ButtonAll, 0, 1);
            tableLayoutPanel3.Dock = DockStyle.Fill;
            tableLayoutPanel3.Location = new Point(111, 3);
            tableLayoutPanel3.Name = "tableLayoutPanel3";
            tableLayoutPanel3.RowCount = 3;
            tableLayoutPanel3.RowStyles.Add(new RowStyle(SizeType.Percent, 16.6677341F));
            tableLayoutPanel3.RowStyles.Add(new RowStyle(SizeType.Percent, 66.66454F));
            tableLayoutPanel3.RowStyles.Add(new RowStyle(SizeType.Percent, 16.6677341F));
            tableLayoutPanel3.Size = new Size(34, 142);
            tableLayoutPanel3.TabIndex = 3;
            // 
            // ButtonAll
            // 
            ButtonAll.BackColor = Color.FromArgb(128, 255, 128);
            ButtonAll.Dock = DockStyle.Fill;
            ButtonAll.Location = new Point(0, 23);
            ButtonAll.Margin = new Padding(0);
            ButtonAll.Name = "ButtonAll";
            ButtonAll.Size = new Size(34, 94);
            ButtonAll.TabIndex = 4;
            ButtonAll.Text = "All";
            ButtonAll.UseVisualStyleBackColor = false;
            ButtonAll.Click += ButtonAll_Click;
            // 
            // ProjectOptions
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            BorderStyle = BorderStyle.FixedSingle;
            Controls.Add(tableLayoutPanel1);
            Name = "ProjectOptions";
            Size = new Size(148, 148);
            tableLayoutPanel1.ResumeLayout(false);
            tableLayoutPanel2.ResumeLayout(false);
            tableLayoutPanel2.PerformLayout();
            tableLayoutPanel3.ResumeLayout(false);
            ResumeLayout(false);
        }

        #endregion
        private Label LabelProjectName;
        private TableLayoutPanel tableLayoutPanel1;
        private TableLayoutPanel tableLayoutPanel2;
        private Button _ButtonMonitor;
        private Button _ButtonFlash;
        private Button _ButtonSetup;
        private Button _ButtonBuild;
        private TableLayoutPanel tableLayoutPanel3;
        private Button ButtonAll;
        private Button buttonCtlBracket;
        private Button buttonUpdateComPort;
        private Button buttonBringToFront;
    }
}
