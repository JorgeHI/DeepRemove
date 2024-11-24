# DeepRemove

![Static Badge](https://img.shields.io/badge/DCC-Nuke-yellow?style=flat) 
![Static Badge](https://img.shields.io/badge/C%2B%2B-_?logo=cplusplus&logoColor=blue&color=grey) 
![Static Badge](https://img.shields.io/badge/Node-Plugin-lightgrey?logo=nuke&logoColor=yellow)
![GitHub Release Date](https://img.shields.io/github/release-date/JorgeHI/DeepRemove)

<picture align="right" width="40%" >
    <source srcset="https://github.com/user-attachments/assets/2756082c-1866-4d0b-8574-e9298e670cb0"  media="(prefers-color-scheme: dark)" align="right" width="40%" >
    <img src="https://github.com/user-attachments/assets/0ccdfb58-7e61-44c2-8165-06a1dc9a576e" align="right" width="40%" >
</picture>
DeepRemove it's a node that's replicates the behavior from the "remove" node from Nuke, but for Deep images. This node allows you to remove or keep an specific list of channels from a deep image input.
Additionally, the design of the propierties panel it's slightly different. The propierties panel it's design to allow the user to add extra channels knobs to selected more channels, instead of a fix number of channel knobs.

<br/><br/><br/><br/>

## Developers example
<picture align="left" width="50%" >
    <source srcset="https://github.com/user-attachments/assets/b92a5c5d-f567-44c3-9d32-78283756b242"  media="(prefers-color-scheme: dark)" align="left" width="50%" >
    <img src="https://github.com/user-attachments/assets/b92a5c5d-f567-44c3-9d32-78283756b242" align="left" width="50%" >
</picture>
This node have been developer for learning purposes and is intended to serve as an aid to those who are entering the NDK. The dynamic system ok knobs for the channels it's a mix of using the dynamic features for knobs provided by the NDK (for the warning knob message) and a basic system of using the HIDDEN flag (show and hide) for the custom channels knobs.

<br/><br/>

The node includes and example of how to show a dynamic text knob that can be useful for warnings, errors or massages that you may want to add for the user without the need to pront a dialog that can be more intrusive.

<p align="center">
  <img src="https://github.com/user-attachments/assets/da4494fd-f15c-407c-afc9-968152bb0fee" />
</p>

<br/><br/><br/><br/>

## Supported Versions
![Static Badge](https://img.shields.io/badge/Nuke-%3D%3D15.1-yellow?style=flat&logo=nuke&logoColor=yellow&logoSize=auto)
![Static Badge](https://img.shields.io/badge/Nuke%20Licence-Commercial-yellow?logo=Nuke&logoColor=yellow)
![Static Badge](https://img.shields.io/badge/C%2B%2B-17-black?logo=cplusplus&logoColor=%2300599C&color=blue)

The node have being developed, tested and compiled for Nuke 15.1 in Windows 11. However, the code does not have any special required that should prevent you to compile it for a different Nuke or OS version.

## TODO list

- [x] Support removal and keep operation options.
- [x] Have a dynamic channel list widget/knobs in the propierties panel.
  - [ ] Make the dynamic channel list work like a dynamic knob system (WIP problem found: [Forum discussion](https://community.foundry.com/discuss/topic/163656/creating-dynamic-knobs-on-node-creation-loading))
- [ ] Create and add a cmake system for the plugin to support easy compiling for Windows and Linux. 

## Author

- [Linkedin](https://www.linkedin.com/in/jorgehi-vfx/)
