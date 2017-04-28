namespace CASP_Standalone_Implementation.Forms
{
    partial class ErrorProviderForm
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
            this.ErrorList = new System.Windows.Forms.ListBox();
            this.Tabs = new System.Windows.Forms.TabControl();
            this.ErrorsTab = new System.Windows.Forms.TabPage();
            this.WarningsTab = new System.Windows.Forms.TabPage();
            this.WarningList = new System.Windows.Forms.ListBox();
            this.Tabs.SuspendLayout();
            this.ErrorsTab.SuspendLayout();
            this.WarningsTab.SuspendLayout();
            this.SuspendLayout();
            // 
            // ErrorList
            // 
            this.ErrorList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ErrorList.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.ErrorList.FormattingEnabled = true;
            this.ErrorList.ItemHeight = 29;
            this.ErrorList.Location = new System.Drawing.Point(3, 3);
            this.ErrorList.Name = "ErrorList";
            this.ErrorList.Size = new System.Drawing.Size(462, 223);
            this.ErrorList.TabIndex = 0;
            // 
            // Tabs
            // 
            this.Tabs.Controls.Add(this.ErrorsTab);
            this.Tabs.Controls.Add(this.WarningsTab);
            this.Tabs.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Tabs.Location = new System.Drawing.Point(0, 0);
            this.Tabs.Name = "Tabs";
            this.Tabs.SelectedIndex = 0;
            this.Tabs.Size = new System.Drawing.Size(476, 258);
            this.Tabs.TabIndex = 1;
            // 
            // ErrorsTab
            // 
            this.ErrorsTab.Controls.Add(this.ErrorList);
            this.ErrorsTab.Location = new System.Drawing.Point(4, 25);
            this.ErrorsTab.Name = "ErrorsTab";
            this.ErrorsTab.Padding = new System.Windows.Forms.Padding(3);
            this.ErrorsTab.Size = new System.Drawing.Size(468, 229);
            this.ErrorsTab.TabIndex = 0;
            this.ErrorsTab.Text = "Errors";
            this.ErrorsTab.UseVisualStyleBackColor = true;
            // 
            // WarningsTab
            // 
            this.WarningsTab.Controls.Add(this.WarningList);
            this.WarningsTab.Location = new System.Drawing.Point(4, 25);
            this.WarningsTab.Name = "WarningsTab";
            this.WarningsTab.Padding = new System.Windows.Forms.Padding(3);
            this.WarningsTab.Size = new System.Drawing.Size(468, 229);
            this.WarningsTab.TabIndex = 1;
            this.WarningsTab.Text = "Warnings";
            this.WarningsTab.UseVisualStyleBackColor = true;
            // 
            // WarningList
            // 
            this.WarningList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.WarningList.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.WarningList.FormattingEnabled = true;
            this.WarningList.ItemHeight = 29;
            this.WarningList.Location = new System.Drawing.Point(3, 3);
            this.WarningList.Name = "WarningList";
            this.WarningList.Size = new System.Drawing.Size(462, 223);
            this.WarningList.TabIndex = 2;
            // 
            // ErrorProviderForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(476, 258);
            this.Controls.Add(this.Tabs);
            this.Name = "ErrorProviderForm";
            this.ShowIcon = false;
            this.Text = "C.A.S.P. Error Output";
            this.Tabs.ResumeLayout(false);
            this.ErrorsTab.ResumeLayout(false);
            this.WarningsTab.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox ErrorList;
        private System.Windows.Forms.TabControl Tabs;
        private System.Windows.Forms.TabPage ErrorsTab;
        private System.Windows.Forms.TabPage WarningsTab;
        private System.Windows.Forms.ListBox WarningList;
    }
}