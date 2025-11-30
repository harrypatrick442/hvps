namespace ProgrammingHelper
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            tableLayoutPanel1 = new TableLayoutPanel();
            projectOptions4 = new ProjectOptions();
            projectOptions3 = new ProjectOptions();
            projectOptions2 = new ProjectOptions();
            projectOptions1 = new ProjectOptions();
            tableLayoutPanel1.SuspendLayout();
            SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            tableLayoutPanel1.ColumnCount = 4;
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 25F));
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 25F));
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 25F));
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 25F));
            tableLayoutPanel1.Controls.Add(projectOptions4, 0, 0);
            tableLayoutPanel1.Controls.Add(projectOptions3, 2, 0);
            tableLayoutPanel1.Controls.Add(projectOptions2, 1, 0);
            tableLayoutPanel1.Controls.Add(projectOptions1, 0, 0);
            tableLayoutPanel1.Dock = DockStyle.Fill;
            tableLayoutPanel1.Location = new Point(0, 0);
            tableLayoutPanel1.Name = "tableLayoutPanel1";
            tableLayoutPanel1.RowCount = 1;
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 100F));
            tableLayoutPanel1.Size = new Size(584, 208);
            tableLayoutPanel1.TabIndex = 0;
            tableLayoutPanel1.Paint += tableLayoutPanel1_Paint;
            // 
            // projectOptions4
            // 
            projectOptions4.BorderStyle = BorderStyle.FixedSingle;
            projectOptions4.Dock = DockStyle.Fill;
            projectOptions4.Location = new Point(149, 3);
            projectOptions4.Name = "projectOptions4";
            projectOptions4.PnpDeviceId = "USB\\VID_10C4&PID_EA60\\0012";
            projectOptions4.ProjectName = "Output Voltage Feedback Module";
            projectOptions4.ProjectRelativePath = "../../../../../C++/OutputVoltageFeedbackModule";
            projectOptions4.Size = new Size(140, 202);
            projectOptions4.TabIndex = 3;
            // 
            // projectOptions3
            // 
            projectOptions3.BorderStyle = BorderStyle.FixedSingle;
            projectOptions3.Dock = DockStyle.Fill;
            projectOptions3.Location = new Point(441, 3);
            projectOptions3.Name = "projectOptions3";
            projectOptions3.PnpDeviceId = "USB\\VID_10C4&PID_EA60\\0001";
            projectOptions3.ProjectName = "Peripheral1";
            projectOptions3.ProjectRelativePath = "../../../../../C++/Peripheral1";
            projectOptions3.Size = new Size(140, 202);
            projectOptions3.TabIndex = 2;
            // 
            // projectOptions2
            // 
            projectOptions2.BorderStyle = BorderStyle.FixedSingle;
            projectOptions2.Dock = DockStyle.Fill;
            projectOptions2.Location = new Point(295, 3);
            projectOptions2.Name = "projectOptions2";
            projectOptions2.PnpDeviceId = "USB\\VID_10C4&PID_EA60\\0011";
            projectOptions2.ProjectName = "First Stage Voltage Feedback Module";
            projectOptions2.ProjectRelativePath = "../../../../../C++/FirstStageVoltageFeedbackModule";
            projectOptions2.Size = new Size(140, 202);
            projectOptions2.TabIndex = 1;
            // 
            // projectOptions1
            // 
            projectOptions1.BorderStyle = BorderStyle.FixedSingle;
            projectOptions1.Dock = DockStyle.Fill;
            projectOptions1.Location = new Point(3, 3);
            projectOptions1.Name = "projectOptions1";
            projectOptions1.PnpDeviceId = "USB\\VID_10C4&PID_EA60\\0033";
            projectOptions1.ProjectName = "HVPS Controller";
            projectOptions1.ProjectRelativePath = "../../../../../C++/HVPSController";
            projectOptions1.Size = new Size(140, 202);
            projectOptions1.TabIndex = 0;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(584, 208);
            Controls.Add(tableLayoutPanel1);
            Name = "Form1";
            Text = "Form1";
            tableLayoutPanel1.ResumeLayout(false);
            ResumeLayout(false);
        }

        #endregion

        private TableLayoutPanel tableLayoutPanel1;
        private ProjectOptions projectOptions3;
        private ProjectOptions projectOptions2;
        private ProjectOptions projectOptions1;
        private ProjectOptions projectOptions4;
    }
}
