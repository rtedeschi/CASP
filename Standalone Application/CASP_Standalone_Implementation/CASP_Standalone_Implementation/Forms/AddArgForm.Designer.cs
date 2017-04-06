namespace CASP_Standalone_Implementation.Forms
{
    partial class AddArgForm
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
            this.ArgNameTextbox = new System.Windows.Forms.TextBox();
            this.AddButton = new System.Windows.Forms.Button();
            this.ArgNameLabel = new System.Windows.Forms.Label();
            this.ArgValueLabel = new System.Windows.Forms.Label();
            this.ArgValueTextbox = new System.Windows.Forms.TextBox();
            this.CancelButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // ArgNameTextbox
            // 
            this.ArgNameTextbox.Location = new System.Drawing.Point(132, 12);
            this.ArgNameTextbox.Name = "ArgNameTextbox";
            this.ArgNameTextbox.Size = new System.Drawing.Size(226, 22);
            this.ArgNameTextbox.TabIndex = 0;
            // 
            // AddButton
            // 
            this.AddButton.Location = new System.Drawing.Point(283, 71);
            this.AddButton.Name = "AddButton";
            this.AddButton.Size = new System.Drawing.Size(75, 26);
            this.AddButton.TabIndex = 2;
            this.AddButton.Text = "Add";
            this.AddButton.UseVisualStyleBackColor = true;
            this.AddButton.Click += new System.EventHandler(this.AddButton_Click);
            // 
            // ArgNameLabel
            // 
            this.ArgNameLabel.AutoSize = true;
            this.ArgNameLabel.Location = new System.Drawing.Point(12, 15);
            this.ArgNameLabel.Name = "ArgNameLabel";
            this.ArgNameLabel.Size = new System.Drawing.Size(114, 17);
            this.ArgNameLabel.TabIndex = 2;
            this.ArgNameLabel.Text = "Argument Name:";
            // 
            // ArgValueLabel
            // 
            this.ArgValueLabel.AutoSize = true;
            this.ArgValueLabel.Location = new System.Drawing.Point(12, 43);
            this.ArgValueLabel.Name = "ArgValueLabel";
            this.ArgValueLabel.Size = new System.Drawing.Size(109, 17);
            this.ArgValueLabel.TabIndex = 4;
            this.ArgValueLabel.Text = "Argument Value";
            // 
            // ArgValueTextbox
            // 
            this.ArgValueTextbox.Location = new System.Drawing.Point(132, 40);
            this.ArgValueTextbox.Name = "ArgValueTextbox";
            this.ArgValueTextbox.Size = new System.Drawing.Size(226, 22);
            this.ArgValueTextbox.TabIndex = 1;
            // 
            // CancelButton
            // 
            this.CancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.CancelButton.Location = new System.Drawing.Point(202, 71);
            this.CancelButton.Name = "CancelButton";
            this.CancelButton.Size = new System.Drawing.Size(75, 26);
            this.CancelButton.TabIndex = 3;
            this.CancelButton.Text = "Cancel";
            this.CancelButton.UseVisualStyleBackColor = true;
            this.CancelButton.Click += new System.EventHandler(this.CancelButton_Click);
            // 
            // AddArgForm
            // 
            this.AcceptButton = this.AddButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.CancelButton;
            this.ClientSize = new System.Drawing.Size(370, 109);
            this.Controls.Add(this.CancelButton);
            this.Controls.Add(this.ArgValueLabel);
            this.Controls.Add(this.ArgValueTextbox);
            this.Controls.Add(this.ArgNameLabel);
            this.Controls.Add(this.AddButton);
            this.Controls.Add(this.ArgNameTextbox);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "AddArgForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "New Argument";
            this.TopMost = true;
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox ArgNameTextbox;
        private System.Windows.Forms.Button AddButton;
        private System.Windows.Forms.Label ArgNameLabel;
        private System.Windows.Forms.Label ArgValueLabel;
        private System.Windows.Forms.TextBox ArgValueTextbox;
        private System.Windows.Forms.Button CancelButton;
    }
}