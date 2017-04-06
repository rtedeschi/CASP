using System.Collections.Generic;

namespace CASP_Standalone_Implementation
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
            this.InputTextbox = new System.Windows.Forms.TextBox();
            this.ModuleCombo = new System.Windows.Forms.ComboBox();
            this.ModuleLabel = new System.Windows.Forms.Label();
            this.RequestTextbox = new System.Windows.Forms.TextBox();
            this.InputLanguageLabel = new System.Windows.Forms.Label();
            this.InputLanguageCombo = new System.Windows.Forms.ComboBox();
            this.ExecuteButton = new System.Windows.Forms.Button();
            this.StatusStrip = new System.Windows.Forms.StatusStrip();
            this.ProgramStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.ShowOutputCheckbox = new System.Windows.Forms.CheckBox();
            this.TitleLabel = new System.Windows.Forms.Label();
            this.StatusStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // InputTextbox
            // 
            this.InputTextbox.AcceptsReturn = true;
            this.InputTextbox.AcceptsTab = true;
            this.InputTextbox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.InputTextbox.Font = new System.Drawing.Font("Courier New", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.InputTextbox.Location = new System.Drawing.Point(12, 72);
            this.InputTextbox.Multiline = true;
            this.InputTextbox.Name = "InputTextbox";
            this.InputTextbox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.InputTextbox.Size = new System.Drawing.Size(1032, 382);
            this.InputTextbox.TabIndex = 0;
            this.InputTextbox.WordWrap = false;
            this.InputTextbox.TextChanged += new System.EventHandler(this.InputTextbox_TextChanged);
            // 
            // ModuleCombo
            // 
            this.ModuleCombo.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.ModuleCombo.FormattingEnabled = true;
            this.ModuleCombo.Location = new System.Drawing.Point(767, 12);
            this.ModuleCombo.Name = "ModuleCombo";
            this.ModuleCombo.Size = new System.Drawing.Size(277, 24);
            this.ModuleCombo.TabIndex = 1;
            this.ModuleCombo.SelectedIndexChanged += new System.EventHandler(this.SelectedIndexChanged);
            // 
            // ModuleLabel
            // 
            this.ModuleLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.ModuleLabel.AutoSize = true;
            this.ModuleLabel.Location = new System.Drawing.Point(703, 15);
            this.ModuleLabel.Name = "ModuleLabel";
            this.ModuleLabel.Size = new System.Drawing.Size(58, 17);
            this.ModuleLabel.TabIndex = 2;
            this.ModuleLabel.Text = "Module:";
            // 
            // RequestTextbox
            // 
            this.RequestTextbox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.RequestTextbox.Location = new System.Drawing.Point(12, 487);
            this.RequestTextbox.Name = "RequestTextbox";
            this.RequestTextbox.ReadOnly = true;
            this.RequestTextbox.Size = new System.Drawing.Size(871, 22);
            this.RequestTextbox.TabIndex = 3;
            // 
            // InputLanguageLabel
            // 
            this.InputLanguageLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.InputLanguageLabel.AutoSize = true;
            this.InputLanguageLabel.Location = new System.Drawing.Point(650, 45);
            this.InputLanguageLabel.Name = "InputLanguageLabel";
            this.InputLanguageLabel.Size = new System.Drawing.Size(111, 17);
            this.InputLanguageLabel.TabIndex = 5;
            this.InputLanguageLabel.Text = "Input Language:";
            // 
            // InputLanguageCombo
            // 
            this.InputLanguageCombo.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.InputLanguageCombo.FormattingEnabled = true;
            this.InputLanguageCombo.Location = new System.Drawing.Point(767, 42);
            this.InputLanguageCombo.Name = "InputLanguageCombo";
            this.InputLanguageCombo.Size = new System.Drawing.Size(277, 24);
            this.InputLanguageCombo.TabIndex = 4;
            this.InputLanguageCombo.SelectedIndexChanged += new System.EventHandler(this.SelectedIndexChanged);
            // 
            // ExecuteButton
            // 
            this.ExecuteButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.ExecuteButton.Location = new System.Drawing.Point(889, 486);
            this.ExecuteButton.Name = "ExecuteButton";
            this.ExecuteButton.Size = new System.Drawing.Size(155, 27);
            this.ExecuteButton.TabIndex = 7;
            this.ExecuteButton.Text = "Execute Command";
            this.ExecuteButton.UseVisualStyleBackColor = true;
            this.ExecuteButton.Click += new System.EventHandler(this.ExecuteButton_Click);
            // 
            // StatusStrip
            // 
            this.StatusStrip.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.StatusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ProgramStatus});
            this.StatusStrip.Location = new System.Drawing.Point(0, 521);
            this.StatusStrip.Name = "StatusStrip";
            this.StatusStrip.Size = new System.Drawing.Size(1056, 22);
            this.StatusStrip.TabIndex = 8;
            this.StatusStrip.Text = "statusStrip1";
            // 
            // ProgramStatus
            // 
            this.ProgramStatus.Name = "ProgramStatus";
            this.ProgramStatus.Size = new System.Drawing.Size(0, 17);
            // 
            // ShowOutputCheckbox
            // 
            this.ShowOutputCheckbox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.ShowOutputCheckbox.AutoSize = true;
            this.ShowOutputCheckbox.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.ShowOutputCheckbox.Location = new System.Drawing.Point(916, 460);
            this.ShowOutputCheckbox.Name = "ShowOutputCheckbox";
            this.ShowOutputCheckbox.Size = new System.Drawing.Size(128, 21);
            this.ShowOutputCheckbox.TabIndex = 9;
            this.ShowOutputCheckbox.Text = "Console Output";
            this.ShowOutputCheckbox.UseVisualStyleBackColor = true;
            // 
            // TitleLabel
            // 
            this.TitleLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.TitleLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TitleLabel.Location = new System.Drawing.Point(12, 9);
            this.TitleLabel.Name = "TitleLabel";
            this.TitleLabel.Size = new System.Drawing.Size(632, 53);
            this.TitleLabel.TabIndex = 10;
            this.TitleLabel.Text = "Code Analyzer\r\nSoftware Project";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1056, 543);
            this.Controls.Add(this.TitleLabel);
            this.Controls.Add(this.ShowOutputCheckbox);
            this.Controls.Add(this.StatusStrip);
            this.Controls.Add(this.ExecuteButton);
            this.Controls.Add(this.InputLanguageLabel);
            this.Controls.Add(this.InputLanguageCombo);
            this.Controls.Add(this.RequestTextbox);
            this.Controls.Add(this.ModuleLabel);
            this.Controls.Add(this.ModuleCombo);
            this.Controls.Add(this.InputTextbox);
            this.MinimumSize = new System.Drawing.Size(1000, 500);
            this.Name = "MainForm";
            this.ShowIcon = false;
            this.Text = "C.A.S.P.";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.StatusStrip.ResumeLayout(false);
            this.StatusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox InputTextbox;
        private System.Windows.Forms.ComboBox ModuleCombo;
        private System.Windows.Forms.Label ModuleLabel;
        private System.Windows.Forms.TextBox RequestTextbox;
        private System.Windows.Forms.Label InputLanguageLabel;
        private System.Windows.Forms.ComboBox InputLanguageCombo;
        private System.Windows.Forms.Button ExecuteButton;
        private System.Windows.Forms.StatusStrip StatusStrip;
        private System.Windows.Forms.ToolStripStatusLabel ProgramStatus;
        private System.Windows.Forms.CheckBox ShowOutputCheckbox;
        private System.Windows.Forms.Label TitleLabel;
    }
}

