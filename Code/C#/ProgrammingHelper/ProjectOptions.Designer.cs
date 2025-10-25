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
            tableLayoutPanel1 = new TableLayoutPanel();
            _ButtonMonitor = new Button();
            _ButtonFlash = new Button();
            _ButtonSetup = new Button();
            _ButtonBuild = new Button();
            LabelProjectName = new Label();
            tableLayoutPanel1.SuspendLayout();
            SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            tableLayoutPanel1.ColumnCount = 1;
            tableLayoutPanel1.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
            tableLayoutPanel1.Controls.Add(_ButtonMonitor, 0, 4);
            tableLayoutPanel1.Controls.Add(_ButtonFlash, 0, 3);
            tableLayoutPanel1.Controls.Add(_ButtonSetup, 0, 1);
            tableLayoutPanel1.Controls.Add(_ButtonBuild, 0, 2);
            tableLayoutPanel1.Dock = DockStyle.Fill;
            tableLayoutPanel1.Location = new Point(0, 0);
            tableLayoutPanel1.Name = "tableLayoutPanel1";
            tableLayoutPanel1.RowCount = 5;
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 20F));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 20.0004959F));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 20.0005F));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 20.0005F));
            tableLayoutPanel1.RowStyles.Add(new RowStyle(SizeType.Percent, 19.9985046F));
            tableLayoutPanel1.Size = new Size(150, 150);
            tableLayoutPanel1.TabIndex = 1;
            // 
            // _ButtonMonitor
            // 
            _ButtonMonitor.BackColor = Color.Fuchsia;
            _ButtonMonitor.Dock = DockStyle.Fill;
            _ButtonMonitor.Location = new Point(0, 120);
            _ButtonMonitor.Margin = new Padding(0);
            _ButtonMonitor.Name = "_ButtonMonitor";
            _ButtonMonitor.Size = new Size(150, 30);
            _ButtonMonitor.TabIndex = 3;
            _ButtonMonitor.Text = "Monitor";
            _ButtonMonitor.UseVisualStyleBackColor = false;
            _ButtonMonitor.Click += _ButtonMonitor_Click;
            // 
            // _ButtonFlash
            // 
            _ButtonFlash.BackColor = Color.FromArgb(255, 192, 128);
            _ButtonFlash.Dock = DockStyle.Fill;
            _ButtonFlash.Location = new Point(0, 90);
            _ButtonFlash.Margin = new Padding(0);
            _ButtonFlash.Name = "_ButtonFlash";
            _ButtonFlash.Size = new Size(150, 30);
            _ButtonFlash.TabIndex = 2;
            _ButtonFlash.Text = "Flash";
            _ButtonFlash.UseVisualStyleBackColor = false;
            _ButtonFlash.Click += _ButtonFlash_Click;
            // 
            // _ButtonSetup
            // 
            _ButtonSetup.BackColor = Color.FromArgb(255, 255, 128);
            _ButtonSetup.Dock = DockStyle.Fill;
            _ButtonSetup.Location = new Point(0, 30);
            _ButtonSetup.Margin = new Padding(0);
            _ButtonSetup.Name = "_ButtonSetup";
            _ButtonSetup.Size = new Size(150, 30);
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
            _ButtonBuild.Location = new Point(0, 60);
            _ButtonBuild.Margin = new Padding(0);
            _ButtonBuild.Name = "_ButtonBuild";
            _ButtonBuild.Size = new Size(150, 30);
            _ButtonBuild.TabIndex = 1;
            _ButtonBuild.Text = "Build";
            _ButtonBuild.UseVisualStyleBackColor = false;
            _ButtonBuild.Click += _ButtonBuild_Click;
            // 
            // LabelProjectName
            // 
            LabelProjectName.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            LabelProjectName.AutoSize = true;
            LabelProjectName.Location = new Point(0, 0);
            LabelProjectName.Margin = new Padding(3);
            LabelProjectName.Name = "LabelProjectName";
            LabelProjectName.Size = new Size(87, 15);
            LabelProjectName.TabIndex = 2;
            LabelProjectName.Text = "[Project Name]";
            // 
            // ProjectOptions
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            Controls.Add(LabelProjectName);
            Controls.Add(tableLayoutPanel1);
            Name = "ProjectOptions";
            tableLayoutPanel1.ResumeLayout(false);
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TableLayoutPanel tableLayoutPanel1;
        private Button _ButtonMonitor;
        private Button _ButtonFlash;
        private Button _ButtonSetup;
        private Button _ButtonBuild;
        private Label LabelProjectName;
    }
}
