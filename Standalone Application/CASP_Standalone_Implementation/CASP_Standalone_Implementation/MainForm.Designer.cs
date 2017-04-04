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
            this.SuspendLayout();
            // 
            // InputTextbox
            // 
            this.InputTextbox.Location = new System.Drawing.Point(12, 140);
            this.InputTextbox.Multiline = true;
            this.InputTextbox.Name = "InputTextbox";
            this.InputTextbox.Size = new System.Drawing.Size(377, 214);
            this.InputTextbox.TabIndex = 0;
            // 
            // ModuleCombo
            // 
            this.ModuleCombo.FormattingEnabled = true;
            this.ModuleCombo.Location = new System.Drawing.Point(157, 33);
            this.ModuleCombo.Name = "ModuleCombo";
            this.ModuleCombo.Size = new System.Drawing.Size(277, 24);
            this.ModuleCombo.TabIndex = 1;
            // 
            // ModuleLabel
            // 
            this.ModuleLabel.AutoSize = true;
            this.ModuleLabel.Location = new System.Drawing.Point(12, 33);
            this.ModuleLabel.Name = "ModuleLabel";
            this.ModuleLabel.Size = new System.Drawing.Size(58, 17);
            this.ModuleLabel.TabIndex = 2;
            this.ModuleLabel.Text = "Module:";
            // 
            // RequestTextbox
            // 
            this.RequestTextbox.Location = new System.Drawing.Point(12, 410);
            this.RequestTextbox.Name = "RequestTextbox";
            this.RequestTextbox.ReadOnly = true;
            this.RequestTextbox.Size = new System.Drawing.Size(886, 22);
            this.RequestTextbox.TabIndex = 3;
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(910, 444);
            this.Controls.Add(this.RequestTextbox);
            this.Controls.Add(this.ModuleLabel);
            this.Controls.Add(this.ModuleCombo);
            this.Controls.Add(this.InputTextbox);
            this.Name = "MainForm";
            this.Text = "CASP";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox InputTextbox;
        private System.Windows.Forms.ComboBox ModuleCombo;
        private System.Windows.Forms.Label ModuleLabel;
        private System.Windows.Forms.TextBox RequestTextbox;
    }
}

