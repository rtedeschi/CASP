namespace CASP_Standalone_Implementation.Forms
{
    partial class CASP_TranslateForm
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
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.splitContainer3 = new System.Windows.Forms.SplitContainer();
            this.SourceLanguageBox = new System.Windows.Forms.TextBox();
            this.SourceDataBox = new System.Windows.Forms.TextBox();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.TargetLanguageBox = new System.Windows.Forms.TextBox();
            this.TargetDataBox = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).BeginInit();
            this.splitContainer3.Panel1.SuspendLayout();
            this.splitContainer3.Panel2.SuspendLayout();
            this.splitContainer3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.splitContainer3);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.splitContainer2);
            this.splitContainer1.Size = new System.Drawing.Size(738, 478);
            this.splitContainer1.SplitterDistance = 363;
            this.splitContainer1.TabIndex = 0;
            // 
            // splitContainer3
            // 
            this.splitContainer3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer3.IsSplitterFixed = true;
            this.splitContainer3.Location = new System.Drawing.Point(0, 0);
            this.splitContainer3.Name = "splitContainer3";
            this.splitContainer3.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer3.Panel1
            // 
            this.splitContainer3.Panel1.Controls.Add(this.SourceLanguageBox);
            // 
            // splitContainer3.Panel2
            // 
            this.splitContainer3.Panel2.Controls.Add(this.SourceDataBox);
            this.splitContainer3.Size = new System.Drawing.Size(363, 478);
            this.splitContainer3.SplitterDistance = 25;
            this.splitContainer3.TabIndex = 1;
            // 
            // SourceLanguageBox
            // 
            this.SourceLanguageBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SourceLanguageBox.Font = new System.Drawing.Font("Courier New", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SourceLanguageBox.Location = new System.Drawing.Point(0, 0);
            this.SourceLanguageBox.Name = "SourceLanguageBox";
            this.SourceLanguageBox.ReadOnly = true;
            this.SourceLanguageBox.Size = new System.Drawing.Size(363, 27);
            this.SourceLanguageBox.TabIndex = 0;
            // 
            // SourceDataBox
            // 
            this.SourceDataBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.SourceDataBox.Font = new System.Drawing.Font("Courier New", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.SourceDataBox.Location = new System.Drawing.Point(0, 0);
            this.SourceDataBox.Multiline = true;
            this.SourceDataBox.Name = "SourceDataBox";
            this.SourceDataBox.ReadOnly = true;
            this.SourceDataBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.SourceDataBox.Size = new System.Drawing.Size(363, 449);
            this.SourceDataBox.TabIndex = 0;
            this.SourceDataBox.WordWrap = false;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.IsSplitterFixed = true;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.TargetLanguageBox);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.TargetDataBox);
            this.splitContainer2.Size = new System.Drawing.Size(371, 478);
            this.splitContainer2.SplitterDistance = 25;
            this.splitContainer2.TabIndex = 0;
            // 
            // TargetLanguageBox
            // 
            this.TargetLanguageBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TargetLanguageBox.Font = new System.Drawing.Font("Courier New", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TargetLanguageBox.Location = new System.Drawing.Point(0, 0);
            this.TargetLanguageBox.Name = "TargetLanguageBox";
            this.TargetLanguageBox.ReadOnly = true;
            this.TargetLanguageBox.Size = new System.Drawing.Size(371, 27);
            this.TargetLanguageBox.TabIndex = 0;
            // 
            // TargetDataBox
            // 
            this.TargetDataBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.TargetDataBox.Font = new System.Drawing.Font("Courier New", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.TargetDataBox.Location = new System.Drawing.Point(0, 0);
            this.TargetDataBox.Multiline = true;
            this.TargetDataBox.Name = "TargetDataBox";
            this.TargetDataBox.ReadOnly = true;
            this.TargetDataBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.TargetDataBox.Size = new System.Drawing.Size(371, 449);
            this.TargetDataBox.TabIndex = 0;
            this.TargetDataBox.WordWrap = false;
            // 
            // CASP_TranslateForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(738, 478);
            this.Controls.Add(this.splitContainer1);
            this.Name = "CASP_TranslateForm";
            this.ShowIcon = false;
            this.Text = "C.A.S.P Translate Module";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.splitContainer3.Panel1.ResumeLayout(false);
            this.splitContainer3.Panel1.PerformLayout();
            this.splitContainer3.Panel2.ResumeLayout(false);
            this.splitContainer3.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer3)).EndInit();
            this.splitContainer3.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel1.PerformLayout();
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.TextBox TargetDataBox;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.TextBox TargetLanguageBox;
        private System.Windows.Forms.SplitContainer splitContainer3;
        private System.Windows.Forms.TextBox SourceLanguageBox;
        private System.Windows.Forms.TextBox SourceDataBox;
    }
}