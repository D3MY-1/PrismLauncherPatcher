# PrismLauncher Patcher

This tool patches the PrismLauncher executable (`prismlauncher.exe`) to enable offline account creation, bypassing the need for a Microsoft account.
A new executable file named `prismlauncher_patched.exe` will be created in the same directory as your original Prism Launcher executable, leaving it untouched.

**Important Information:**

* This patcher works with both installed and portable versions of Prism Launcher.
* Future updates to Prism Launcher may break the functionality of this patcher.
* Different patching methods are used for installed and portable versions to ensure compatibility.
* This tool has been tested with **Prism Launcher version 9.2**. Compatibility with other versions is not guaranteed.


---

**How to Use:**

1.  **Download:** Obtain the latest release of the PrismLauncher Patcher from the [Releases](https://github.com/D3MY-1/PrismLauncherPatcher/releases) page.
2.  **Placement:** Place the downloaded `PrismLauncherPatcher.exe` file in the **same directory as your Prism Launcher's `prismlauncher.exe` file.**
3.  **Execution:** Run the `PrismLauncherPatcher.exe`. The patcher will automatically detect your Prism Launcher installation type and apply the correct patch.
4.  **Result:** A new file named `prismlauncher_patched.exe` will be created in the same directory. This is your patched version of Prism Launcher.


**Launching the Game:**

1.  **Run the Patched Launcher:** Launch Prism Launcher by running `prismlauncher_patched.exe` instead of `prismlauncher.exe`.
2.  **Create Offline Account:** You should now be able to create an offline account within the patched application.
3.  **Launch Instance (Potential Microsoft Account Prompt):** When launching a Minecraft instance, you might encounter a window with the message: "`In order to play Minecraft, you must have at least one Microsoft account that owns Minecraft logged in. Would you like to open the account manager to add an account now?`"
4.  **Workaround:** If you see this prompt, simply click the **"Yes"** button on the prompt window and then **close the PrismLauncher Account manager window** that appears. Minecraft should then proceed to launch correctly.
