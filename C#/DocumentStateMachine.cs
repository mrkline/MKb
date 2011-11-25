using System.Windows.Forms;

public abstract class DocumentStateMachine
{
	public string Filepath { get; private set; }

    public string Filename
    {
        get
        {
            if (Filepath == null)
                return null;

            return System.IO.Path.GetFileNameWithoutExtension(Filepath);
        }
    }

	/// <summary>
	/// True if the file has been modified since it was last saved
	/// </summary>
	private bool modded;
    public bool Modified
	{
		get { return modded; }
		set
		{
			bool prev = modded;
			modded = value;
			if (prev != value)
				OnChange();
		}
	}

	public DocumentStateMachine()
	{
		modded = false;
		Filepath = null;
	}

    /// <summary>
    /// Called when the file is changed, opened, etc. Derived classes may want to
    /// perform actions such as showing the new file name in the form's title.
    /// </summary>
    public virtual void OnChange() { }

    public void OnNew()
    {
        if (!PromptAndSaveChanges())
            return;
        Filepath = null;
        Reset();
		Modified = false;
		OnChange();
    }

    public void OnOpen()
    {
        if (!PromptAndSaveChanges())
            return;

        string newFilepath = ShowOpenDialog();

        if (newFilepath != null)
        {
            Filepath = newFilepath;
            Open();
			Modified = false;
			OnChange();
        }
    }

    public void OnSave()
    {
        if (Filepath == null)
            Filepath = ShowSaveDialog();

		if (Filepath != null)
		{
			Save();
			Modified = false;
			OnChange();
		}
    }

    public void OnSaveAs()
    {
        string newFilepath = ShowSaveDialog();

        if (newFilepath != null)
        {
            Filepath = newFilepath;
            Save();
			Modified = false;
			OnChange();
        }
    }

    /// <summary>
    /// If changes have been made since the last save point, prompts the user
    /// to save changes, and saves if needed
    /// </summary>
    /// <returns>
    /// true to continue on, false if the current action should be cancelled
    /// </returns>
    public bool PromptAndSaveChanges()
    {
        if (Modified)
        {
            switch (MessageBox.Show(
                "The current file has been modified since you last saved. Save changes?",
                "Save changes?", MessageBoxButtons.YesNoCancel,
                MessageBoxIcon.Question))
            {
                case DialogResult.Yes:
                    if (Filepath == null)
                    {
                        Filepath = ShowSaveDialog();
                        if(Filepath == null)
                            return false;
                    }
                    else
                    {
                        Save();
                    }
                    break;

               case DialogResult.Cancel:
                    return false;
            }
        }
        return true;
    }

    /// <summary>
    /// Displays a File Open dialog
    /// </summary>
    /// <returns>
    /// The new filepath, or null if the user cancelled the dialog
    /// </returns>
    public abstract string ShowOpenDialog();

    /// <summary>
    /// Displays a File Save dialog
    /// </summary>
    /// <returns>
    /// The new filepath, or null if hte user cancelled the dialog.
    /// </returns>
    public abstract string ShowSaveDialog();

    /// <summary>
    /// Opens the file at Filepath
    /// </summary>
    /// <returns></returns>
    public abstract void Open();

    /// <summary>
    /// Saves the current file at the location specified by Filepath
    /// </summary>
    public abstract void Save();

    /// <summary>
    /// Resets the current display for a new file
    /// </summary>
    public abstract void Reset();
};