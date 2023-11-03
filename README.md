# L86Duntu files

This repo contains a snapshot of the home directory of the default user in [L86Duntu](https://www.86duino.com/?p=8923), the Linux 'distribution' made available by DMP Electronics for their '86Duino' SBC boards.

Though an [open source Arduino-alike SDK is available](https://github.com/roboard/86Duino), and a more [Linux-oriented SDK](https://github.com/roboard/86Duino_Linux_SDK), they do not seem to contain all the files here, notably the `86adc`, `86clock`, `86comsh`, `86crsbar`, `86info` and `86io` tools, which in the SDK are only compiled for use in DOS.

These tools do work on recent linux - tested on Gentoo:

```
jamie@86duino ~/L86Duntu Files $ cat /proc/version
Linux version 6.1.57-gentoo (root@86duino) (i486-pc-linux-gnu-gcc (Gentoo 13.2.1_p20230826 p7) 13.2.1 20230826, GNU ld (Gentoo 2.40 p5) 2.40.0) #6 Fri Nov  3 02:42:51 PDT 2023
```

I suspect they will also be useful to people running other Vortex86EX-based hardware.
