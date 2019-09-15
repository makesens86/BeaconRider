# *BeaconRider*
[*BeaconRider*](http://network.snu.ac.kr:14259/webdav/hp_publications/paper/BeaconRider_icnp2019_hjlee.pdf) is an opportunistic air-time sharing scheme that enables simultaneous data and beacon transmissions aimed at improving spectrum efficiency in dense network environments. If you reference our work, please refer to the following paper information. 

Hyunjoong Lee, Jungjun Kim, Changhee Joo, and Saewoong Bahk, "BeaconRider: Opportunistic Sharing of Beacon Air-Time in Densely Deployed WLANs," in Proc. in *IEEE ICNP 2019*, Chicago, USA, Oct. 7-10, 2019.

We provide the source codes (device driver and firmware codes) of BeaconRider. It has been tested under following development environments. 

- OS: Ubuntu 14.04
- AP installation: [*hostapd-2.7*](https://w1.fi/hostapd/)
- Device Driver: Linux Kernel Backports 4.2.6-1, *ath9k*
- Device Firmware: [*ath9k-htc*](https://github.com/makesens86/open-ath9k-htc-firmware)
- Tested Device: [TP-Link WN722N version 1 (v1)](https://wikidevi.com/wiki/TP-LINK_TL-WN722N): Note that the newer versions (v2 and v3) is not compatible with BeaconRider because it no longer uses an atheros chipset. Make sure that you to buy the v1 version. 

If you have any questions, feel free to contact us. 

- E-mail: hjlee@netlab.snu.ac.kr 

# Source code description
- *drivers*: Linux kernel driver
Most of *BeaconRider* operations are implemented in ath9k-htc firmware (beaconRider-ath9k-htc-firmware/target_firmware/wlan). 
- *target_firmware*: ath9k-htc firmware

# How to build *ath9k-htc* firmware
1. Download firmware source code from [here](https://github.com/makesens86/open-ath9k-htc-firmware).

```git clone https://github.com/qca/open-ath9k-htc-firmware```

2. Download and install *toolchain* using *make* command. (It will take about half an hour to be completed.)
```
cd ./open-ath9k-htc-firmware
make toolchain
```

3. Building firmware
```
sudo apt-get install cmake
make -C target_firmware
```
If you can find *htc_9271.fw* in open-ath9k-htc-firmware/target_firmware, the building is successful.

4. Replace the new firmware with the old one. Remove old kernel module (*ath9k-htc*) and reinstall new one. 
```
sudo cp target_firmware/htc_9271.fw /lib/firmware/ath9k_htc/htc_9271-1.4.0.fw
sudo rmmod ath9k_htc
sudo modprobe ath9k_htc
```
If you have an issue reinstalling new kernel module, reboot your system. 
