namespace CASP_Standalone_Implementation.Forms
{
    partial class CASP_OutlineForm
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
            this.FlowPanel = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // FlowPanel
            // 
            this.FlowPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.FlowPanel.Location = new System.Drawing.Point(0, 0);
            this.FlowPanel.Name = "FlowPanel";
            this.FlowPanel.Size = new System.Drawing.Size(738, 478);
            this.FlowPanel.TabIndex = 0;
            // 
            // CASP_OutlineForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(738, 478);
            this.Controls.Add(this.FlowPanel);
            this.Name = "CASP_OutlineForm";
            this.Text = "CASP_OutlineForm";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel FlowPanel;
    }
}