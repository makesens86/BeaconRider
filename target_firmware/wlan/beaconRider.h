#ifndef BEACONRIDER_H
#define BEACONRIDER_H

#include <adf_os_types.h>
#include <adf_os_pci.h>
#include <adf_os_dma.h>
#include <adf_os_timer.h>
#include <adf_os_lock.h>
#include <adf_os_io.h>
#include <adf_os_mem.h>
#include <adf_os_util.h>
#include <adf_os_stdtypes.h>
#include <adf_os_defer.h>
#include <adf_os_atomic.h>
#include <adf_nbuf.h>
#include <adf_net.h>
#include <adf_net_wcmd.h>
#include <adf_os_irq.h>

#include <if_ath_pci.h>
#include "if_llc.h"
#include "ieee80211_var.h"
#include "if_athrate.h"
#include "if_athvar.h"
#include "ah_desc.h"
#include "ah.h"
#include "ar5416desc.h"                           
#include "ar5416reg.h"
#define ar5416_desc ar5416_desc_20 
#define AR5416DESC         AR5416DESC_20
#define ath_tgt_free_skb  adf_nbuf_free

#ifdef USE_HEADERLEN_RESV
#include <if_llc.h>
#endif

#define OFDM_PLCP_BITS          22
#define HT_RC_2_MCS(_rc)        ((_rc) & 0x0f)
#define HT_RC_2_STREAMS(_rc)    ((((_rc) & 0x78) >> 3) + 1)
#define L_STF                   8
#define L_LTF                   8
#define L_SIG                   4
#define HT_SIG                  8
#define HT_STF                  4
#define HT_LTF(_ns)             (4 * (_ns))
#define SYMBOL_TIME(_ns)        ((_ns) << 2)            // ns * 4 us
#define SYMBOL_TIME_HALFGI(_ns) (((_ns) * 18 + 4) / 5)  // ns * 3.6 us

#include "attacks.h"
#include "if_owl.h"

int beaconRider_confRadio(struct ath_softc_tgt *sc);
int beaconRider_restoreRadio(struct ath_softc_tgt *sc);
int beaconRider_isRideeBeacon(struct ath_softc_tgt *sc);
int beaconRider_txPacket(struct ath_softc_tgt *sc);
int beaconRider_makeRidingPacket(struct ath_softc_tgt *sc, struct ath_txq *txq);
void beaconRider_printAddr(volatile unsigned char *buff);
void beaconRider_setPM(unsigned char *buff);
void beaconRider_setSRC(struct ath_hal *ah, unsigned char *buff);

int beaconRider_findBeaconIndex(struct ath_softc_tgt *sc, unsigned char *bssid);
void beaconRider_updateBeaconList(struct ath_softc_tgt *sc, struct ath_rx_desc *ds, unsigned char *bssid, volatile unsigned char *buff);
void beaconRider_printBeaconList(struct ath_softc_tgt *sc);

int beaconRider_getBeaconLength(struct ath_softc_tgt *sc, unsigned char *bssid);
//inline unsigned int update_elapsed(unsigned int prev, unsigned int freq, unsigned int *elapsed);

#endif // BEACONRIDER_H

