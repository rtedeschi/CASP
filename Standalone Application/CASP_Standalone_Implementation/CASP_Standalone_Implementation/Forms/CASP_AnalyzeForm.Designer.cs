namespace CASP_Standalone_Implementation.Forms
{
    partial class CASP_AnalyzeForm
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
            this.Parent = new System.Windows.Forms.Panel();
            this.complexitylabel = new System.Windows.Forms.Label();
            this.fnlabel = new System.Windows.Forms.Label();
            this.Parent.SuspendLayout();
            this.SuspendLayout();
            // 
            // Parent
            // 
            this.Parent.AutoScroll = true;
            this.Parent.Controls.Add(this.complexitylabel);
            this.Parent.Controls.Add(this.fnlabel);
            this.Parent.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Parent.Location = new System.Drawing.Point(0, 0);
            this.Parent.Name = "Parent";
            this.Parent.Size = new System.Drawing.Size(738, 478);
            this.Parent.TabIndex = 1;
            // 
            // complexitylabel
            // 
            this.complexitylabel.Font = new System.Drawing.Font("Courier New", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.complexitylabel.Location = new System.Drawing.Point(375, 9);
            this.complexitylabel.Name = "complexitylabel";
            this.complexitylabel.Size = new System.Drawing.Size(351, 36);
            this.complexitylabel.TabIndex = 1;
            this.complexitylabel.Text = "Calculated Complexity";
            this.complexitylabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // fnlabel
            // 
            this.fnlabel.Font = new System.Drawing.Font("Courier New", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.fnlabel.Location = new System.Drawing.Point(12, 9);
            this.fnlabel.Name = "fnlabel";
            this.fnlabel.Size = new System.Drawing.Size(351, 36);
            this.fnlabel.TabIndex = 0;
            this.fnlabel.Text = "Function Title";
            this.fnlabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // CASP_AnalyzeForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(738, 478);
            this.Controls.Add(this.Parent);
            this.Name = "CASP_AnalyzeForm";
            this.ShowIcon = false;
            this.Text = "C.A.S.P Analyze Module";
            this.Parent.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Label complexitylabel;
        private System.Windows.Forms.Label fnlabel;
        private System.Windows.Forms.Panel Parent;
    }
}