# Line Following Robot

This repository contains the Arduino sketches and configuration files for a Line Following Robot project.

Contents
--------

- `fastLineFollow/arduino_secrets.h` (ignored — contains secrets/local configuration)
- `fastLineFollow/arduino_secrets.h.example` (optional template for the secrets file)
- `sketch.json` (project metadata)
- `fastLineFollowRBT/fastLineFollowRBT.ino` (main Arduino sketch)

Getting started
---------------

1. Install the Arduino IDE (or PlatformIO) and the required board support for your microcontroller.
2. Copy `fastLineFollow/arduino_secrets.h.example` to `fastLineFollow/arduino_secrets.h` and fill in any local credentials or pins. Do not commit the real `arduino_secrets.h`.
3. Open `fastLineFollowRBT/fastLineFollowRBT.ino` in the Arduino IDE.
4. Select the correct board and serial port, then upload.

Security / Secrets
------------------

Do NOT commit `arduino_secrets.h` with real credentials. Add this line to your `.gitignore`:

```
fastLineFollow/arduino_secrets.h
```

How to push this project to GitHub
---------------------------------

If this folder is not yet a git repository, run:

```powershell
git init
git add .
git commit -m "Initial commit: Line Following Robot"
git remote add origin https://github.com/tashindudasanayake/Line_Following_Robot.git
git branch -M main
git push -u origin main
```

If the remote already has commits, fetch and rebase first:

```powershell
git remote add origin https://github.com/tashindudasanayake/Line_Following_Robot.git
git fetch origin
git pull --rebase origin main
git add .
git commit -m "Add project files"
git push -u origin main
```

License
-------

Choose a license and add a `LICENSE` file if you want to make this project open source.

Contact
-------

If you need help setting up or uploading the sketch, open an issue or contact the maintainer.
