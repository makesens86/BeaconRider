#include "beaconRider.h"

/*************************/
/* BeaconRider Functions */
/*************************/


int beaconRider_confRadio(struct ath_softc_tgt *sc)
{

    struct ath_hal *ah = sc->sc_ah;
    int q;

    ah->tempSIFS = ioread32_mac(AR_D_GBL_IFS_SIFS);
    ah->tempSLOT = ioread32_mac(AR_D_GBL_IFS_SLOT);
    ah->tempEIFS = ioread32_mac(AR_D_GBL_IFS_EIFS);

    // Clock Rate: 44
    // SIFS: 352 = 8 * 44 -> 8 us
    // SLOT: 880 = 20 * 44 -> 20 us
    // AIFSN: 7
    
    //printk("Current SIFS, SLOT, EIFS, BACKOFF: ");    
    //printk(itos(ioread32_mac(AR_D_GBL_IFS_SIFS)));
    //printk(", ");
    //printk(itos(ioread32_mac(AR_D_GBL_IFS_SLOT)));
    //printk(", ");
    //printk(itos(ioread32_mac(AR_D_GBL_IFS_EIFS)));
    //printk(", ");
    //printk(itos(ioread32_mac(AR_DLCL_IFS(0))));
    //printk("\n");
    
   
    /* Ignore physical and virtual carrier sensing */
    iowrite32_mac(AR_DIAG_SW, ioread32_mac(AR_DIAG_SW)
            | AR_DIAG_FORCE_RX_CLEAR | AR_DIAG_IGNORE_VIRT_CS);

    /*  Set SIFS to small value - ath9k_hw_set_sifs_time */
    iowrite32_mac(AR_D_GBL_IFS_SIFS, 1);

    /*  Set slot time to value - ath9k_hw_setslottime */
    iowrite32_mac(AR_D_GBL_IFS_SLOT, 1);

    /*  Set EIFS to small value - ath9k_hw_set_eifs_timeout */
    iowrite32_mac(AR_D_GBL_IFS_EIFS, 1);

    /* Disable backoff behaviour by setting parameters to zero */
    for (q = 0; q < HAL_NUM_TX_QUEUES; q++) {
        ah->tempIFS[q] = ioread32_mac(AR_DLCL_IFS(q));
        /* Reset CW_MIN, CW_MAX, and AIFSN for every transmit queue */
        iowrite32_mac(AR_DLCL_IFS(q), 0);
    }

    /*
    printk("After SIFS, SLOT, EIFS, BACKOFF: ");    
    printk(itos(ioread32_mac(AR_D_GBL_IFS_SIFS)));
    printk(", ");
    printk(itos(ioread32_mac(AR_D_GBL_IFS_SLOT)));
    printk(", ");
    printk(itos(ioread32_mac(AR_D_GBL_IFS_EIFS)));
    printk(", ");
    printk(itos(ioread32_mac(AR_DLCL_IFS(0))));
    printk("\n");
*/

    // Alternative / additional interesting registers:
    // - AR_D_GBL_IFS_MISC to disable backoff (and other optimizations)
    // - AR_DMISC to disable backoff for each queue
    // - AR_DCHNTIME to set unlimited channel time for each queue
    // - AR_Q_TXD to disable Tx on other queues (see datasheet for usage)
    // - AR_DMISC to disable post backoff and virtual collision handling

    return 0;
}

int beaconRider_restoreRadio(struct ath_softc_tgt *sc)
{
    struct ath_hal *ah = sc->sc_ah;
    int q;

    /* Restore physical and virtual carrier sensing */
    iowrite32_mac(AR_DIAG_SW, ioread32_mac(AR_DIAG_SW) & ~AR_DIAG_FORCE_RX_CLEAR);
    iowrite32_mac(AR_DIAG_SW, ioread32_mac(AR_DIAG_SW) & ~AR_DIAG_IGNORE_VIRT_CS);

    /*  Set SIFS to small value - ath9k_hw_set_sifs_time */
    iowrite32_mac(AR_D_GBL_IFS_SIFS, ah->tempSIFS);

    /*  Set slot time to value - ath9k_hw_setslottime */
    iowrite32_mac(AR_D_GBL_IFS_SLOT, ah->tempSLOT);

    /*  Set EIFS to small value - ath9k_hw_set_eifs_timeout */
    iowrite32_mac(AR_D_GBL_IFS_EIFS, ah->tempEIFS);

    /* Disable backoff behaviour by setting parameters to zero */
    for (q = 0; q < HAL_NUM_TX_QUEUES; q++) {
        /* Reset CW_MIN, CW_MAX, and AIFSN for every transmit queue */
        iowrite32_mac(AR_DLCL_IFS(q), ah->tempIFS[q]);
    }
    /*
    printk("Restore SIFS, SLOT, EIFS, BACKOFF: ");    
    printk(itos(ioread32_mac(AR_D_GBL_IFS_SIFS)));
    printk(", ");
    printk(itos(ioread32_mac(AR_D_GBL_IFS_SLOT)));
    printk(", ");
    printk(itos(ioread32_mac(AR_D_GBL_IFS_EIFS)));
    printk(", ");
    printk(itos(ioread32_mac(AR_DLCL_IFS(0))));
    printk("\n");
*/
    // Alternative / additional interesting registers:
    // - AR_D_GBL_IFS_MISC to disable backoff (and other optimizations)
    // - AR_DMISC to disable backoff for each queue
    // - AR_DCHNTIME to set unlimited channel time for each queue
    // - AR_Q_TXD to disable Tx on other queues (see datasheet for usage)
    // - AR_DMISC to disable post backoff and virtual collision handling

    return 0;
}


void beaconRider_printTime(void)
{
    printk("[");
    printk(itos(NOW()/117));
    printk("] ");
    return;
}

/*
void beaconRider_printSrc(volatile unsigned char *buff)
{
    int i;
    printk("CUR SRC: ");
    for (i = 0; i < 5 ; i++)
    {
        printk(itox(buff[10 + i]));
        printk(":");
    }
    printk(itox(buff[15]));
    printk("\n");

    return;
}
*/
void beaconRider_printAddr(volatile unsigned char *buff)
{
    // print BSSID from received buffer
    int i;
    printk("CUR BSSID: ");
    for (i = 0; i < 5 ; i++)
    {
        printk(itox(buff[16 + i]));
        printk(":");
    }
    printk(itox(buff[21]));
    printk("\n");

    return;
}
/*
void beaconRider_printBssid(volatile unsigned char *bssid)
{
    int i;
    printk("CUR BSSID: ");
    for (i = 0; i < 5 ; i++)
    {
        printk(itox(bssid[i])); // print BSSID, not SRC Address
        printk(":");
    }
    printk(itox(bssid[5]));
    printk("\n");

    return;

}
*/
void beaconRider_printBeaconList(struct ath_softc_tgt *sc)
{

    struct ath_hal *ah = sc->sc_ah;               
    int i, j;

    for (i = 0; i < ah->numBeacon; i++)
    {
        printk("Index: ");
        printk(itos(i));
        printk(", Length: ");
        printk(itos(ah->beaconList[i].length));
        printk(", Rssi: ");
        printk(itos(ah->beaconList[i].rssi));
        printk(", Count: ");
        printk(itos(ah->beaconList[i].count));
        printk(", canRide: ");
        printk(itos(ah->beaconList[i].canRide));
        printk(", CUR MAC: ");
        for (j = 0; j < 5; j++)
        {
            printk(itox(ah->beaconList[i].bssid[j]));
            printk(":");
        }
        printk(itox(ah->beaconList[i].bssid[5]));
        printk("\n");
    }

}

int beaconRider_isPacketBuffered(struct ath_softc_tgt *sc)
{
    struct ath_txq *txq;
    struct ath_atx_tid  *tid;

    a_int32_t i;

    for (i = 0; i < (HAL_NUM_TX_QUEUES - 6); i++) 
    {    
        txq = ATH_TXQ(sc, i);                     
        tid = asf_tailq_first(&txq->axq_tidq);

        if (tid == NULL) continue;

        if(!asf_tailq_empty(&tid->buf_q))
            return 1;   // If there is buffered packet to transmit
    }  
    return 0;
}

void beaconRider_setPMbit(unsigned char *buff)    
{
    // set Power Management bit for beaconRider indication
    buff[1] += 0x10;
}


void beaconRider_setMDbit(unsigned char *buff)    
{
    // set More Data bit for buffered data indication  
    buff[1] += 0x20;
}

void beaconRider_unsetMDbit(unsigned char *buff)    
{
    // set More Data bit for buffered data indication  
    buff[1] -= 0x20;
}


int beaconRider_isPMset(unsigned char *buff)
{
    if(buff[1] & 0x10)  return 1;
    return 0;
}

int beaconRider_isMDset(unsigned char *buff)
{
    if(buff[1] & 0x20)  return 1;
    return 0;
}

int beaconRider_isSrcSet(unsigned char *buff)
{
    int isSrcSet = 0;
    int i;

    for (i = 0; i < 6; i++){
        if (buff[10+i] != buff[16+i])
            isSrcSet = 1;
    }

    return isSrcSet;
}

/*
void beaconRider_setDuration(unsigned char *buff)    
{
    // set More Data bit for buffered data indication  
    buff[2] += 0x90;
    buff[3] += 0x7F;
}
*/

/*
void beaconRider_setTim(unsigned char *buff)    
{
    // set More Data bit for buffered data indication  
    buff[62] = 0xFF;
}
*/
void beaconRider_setSRC(struct ath_hal *ah, unsigned char *buff)
{
    int i;
    int beaconIndex = -1;

    if (ah->numBeacon == 0) return 0;   // if there is no rider candidate, do nothing 

    unsigned int min_rssi = 10000;  // 100 * 100  
    // Rider selection 
    for (i = 0; i < ah->numBeacon; i++)           
    {                                             
        if(ah->beaconList[i].canRide && ah->beaconList[i].rssi < min_rssi)
        {
            min_rssi = ah->beaconList[i].rssi;
            beaconIndex = i;
        }
    }

    if (beaconIndex == -1)
    {
        // If there is no rider candidate, Do nothing
        /*
        for (i = 0; i < 6; i++)
        {
            buff[10+i] = ah->sourceAddress[i]; // save rider address to src address field
        }
        */
        //printk("No Rider is selected\n");
        ah->isSrcSet = 0;
    }
    else
    {
        /*
        printk("Rider AP: [");
        printk(itox(ah->beaconList[beaconIndex].bssid[5]));
        printk("]\n");
        */
        // Save rider adddress
        for (i = 0; i < 6; i++)
        {
            ah->riderAddress[i] = ah->beaconList[beaconIndex].bssid[i];
            buff[10+i] = ah->riderAddress[i]; // save rider address to src address field
        }

        ah->isSrcSet = 1;
    }
}

int beaconRider_findBeaconIndex(struct ath_softc_tgt *sc, unsigned char *bssid)
{
    //printk("beaconRider_findBeaconIndex\n");
    struct ath_hal *ah = sc->sc_ah;
    int i;

    /*
       printk("Current numBeacon: ");
       printk(itos(ah->numBeacon));
       printk("\n");
    */
    for (i = 0; i < ah->numBeacon; i++)
    {
        if (A_MEMCMP(ah->beaconList[i].bssid, bssid, 6) == 0)
            return i;
    }
    return -1;
}

void beaconRider_updateBeaconList(struct ath_softc_tgt *sc, struct ath_rx_desc *ds, unsigned char *bssid, volatile unsigned char *buff)
{
    //printk("beaconRider_updateBeaconList\n");
    struct ath_hal *ah = sc->sc_ah;
    int i;

    struct ar5416_desc ads;
    struct ar5416_desc *adsp = AR5416DESC(ds);
    ads.u.rx = adsp->u.rx;
    unsigned int beaconLength = ads.ds_rxstatus1 & AR_DataLen;
    //unsigned int beaconRssi = MS(ads.ds_rxstatus4, AR_RxRSSICombined);
    unsigned int beaconRssi = MS(ads.ds_rxstatus0, AR_RxRSSIAnt00);

    int beaconIndex = beaconRider_findBeaconIndex(sc, bssid);
   
    // Rssi moving average factor
    int weight = 125;
    int total = 1000;
    int digit = 100;
    int effectiveRssiCount = 10;        // Minimum number of necessary beacons
   
    /*
    printk("beaconIndex: ");
    printk(itos(beaconIndex));
    printk(", NumBeacon: ");
    printk(itos(ah->numBeacon));
    printk("\n");
    */
    
    if (beaconIndex == -1)    // When it's a new beacon
    {
        if (ah->numBeacon == NUM_MAX_AP)   
        {
            printk("BeaconList MEM Short!\n");
            return 0;

        }

        //A_MEMCPY(ah->beaconList[ah->numBeacon].bssid, buff + 10, 6);
        for (i = 0; i < 6; i++)
        {
            ah->beaconList[ah->numBeacon].bssid[i] = bssid[i];
        }
        ah->beaconList[ah->numBeacon].length = beaconLength;
        ah->beaconList[ah->numBeacon].rssi = beaconRssi * digit;
        ah->beaconList[ah->numBeacon].buffer = 0;
        if(beaconRider_isMDset(buff))
        {
            // if there is buffered frames in the AP
            ah->beaconList[ah->numBeacon].buffer = 1;
        }
        ah->beaconList[ah->numBeacon].count++;
        ah->beaconList[ah->numBeacon].canRide = 0;

        ah->numBeacon++;
        
        /*
        printk("New beacon is added! numBeacon: ");
        printk(itos(ah->numBeacon));
        printk("\n");
        */
    }
    else
    {

        unsigned int rssi_prev, rssi_new;

        rssi_prev = ah->beaconList[beaconIndex].rssi;
        rssi_new = beaconRssi * digit;

        if (ah->beaconList[beaconIndex].count < effectiveRssiCount)
            ah->beaconList[beaconIndex].rssi = rssi_prev + rssi_new;  // Before it reaches to effectiveRssiCount it just adds
        else
            ah->beaconList[beaconIndex].rssi = ( (total - weight) * rssi_prev + (weight * rssi_new) ) / total;

        ah->beaconList[beaconIndex].buffer = 0;
        if(beaconRider_isMDset(buff))
        {
            ah->beaconList[beaconIndex].buffer = 1;
        }
        ah->beaconList[beaconIndex].count++;

        // Calculate Average Rssi based on first 10 samples
        if(ah->beaconList[beaconIndex].count == effectiveRssiCount)
            ah->beaconList[beaconIndex].rssi = ah->beaconList[beaconIndex].rssi / effectiveRssiCount;

        if ((ah->beaconList[beaconIndex].buffer == 1) && (ah->beaconList[beaconIndex].count > effectiveRssiCount) )
        {
            //printk("This AP can Ride\n");
            ah->beaconList[beaconIndex].canRide = 1;
        }else{
            ah->beaconList[beaconIndex].canRide = 0;
        }
    }
    return 0;
}

int beaconRider_getBeaconLength(struct ath_softc_tgt *sc, unsigned char *bssid)
{
    //printk("beaconRider_getBeaconLength\n");
    struct ath_hal *ah = sc->sc_ah;
    int beaconIndex = beaconRider_findBeaconIndex(sc, bssid);

    if (beaconIndex == -1)
    {
        //printk("The beacon is not enlisted\n");
        return -1;
    }
    else
        return ah->beaconList[beaconIndex].length;
}

int beaconRider_mcsToKbps (int idx) // 20 MHz, LGI
{
    switch (idx) {
        case 0:
            return 6500;
        case 1:
            return 13000;
        case 2:
            return 19500;
        case 3:
            return 26000;
        case 4:
            return 39000;
        case 5:
            return 52000;
        case 6:
            return 58500;
        case 7:
            return 65000;
        default:
            //printk("mcsToKbps: Invalid MCS index\n");
            return -1;
    }
}

int beaconRider_rixToMcs(int Rix)
{
    switch (Rix) {
        case 12:
            return 0;
        case 13:
            return 1;
        case 14:
            return 2;
        case 15:
            return 3;
        case 16:
        case 17:
            return 4;
        case 18:
        case 19:
            return 5;
        case 20:
        case 21:
            return 6;
        case 22:
        case 23:
            return 7;
        default:
            /*
            printk("rixToMcs: Invalid MCS index: [");
            printk(itos(Rix));
            printk("]\n");
            */
            return -1;
    }
}

int beaconRider_mcsToRix(int mcs)
{
    switch (mcs) {
        case 0:
            return 12;
        case 1:
            return 13;
        case 2:
            return 14;
        case 3:
            return 15;
        case 4:
            return 16;
        case 5:
            return 18;
        case 6:
            return 20;
        case 7:
            return 22;
        default:
            //printk("mcsToRix: Invalid MCS index\n");
            return -1;
    }
}

int beaconRider_checkRidable(struct ath_softc_tgt *sc) 
{ 
    struct ath_hal *ah = sc->sc_ah; 

    int i;      // iterator for MCS index 
    int thisMcs, bestMcs, maxRix, minRix, maxMcs, minMcs; 
    int thisUtility, bestUtility; 
    int thisRateKbps, thisSfer; 
    int rideeIndex = beaconRider_findBeaconIndex(sc, ah->rideeAddress); 

    maxRix = ah->chosenRix;         // Selected Rix from rate adaptation algorithm 
    maxMcs = beaconRider_rixToMcs(maxRix); 
    minMcs = 3; 
    thisUtility = 0; 
    bestUtility = -9999999999; 
    bestMcs = minMcs; 

    for (i = maxMcs; i >= minMcs; i--) 
    { 
        thisMcs = i; 
        thisRateKbps = beaconRider_mcsToKbps(thisMcs); 
        thisSfer = ah->sferTable[rideeIndex].sfer[maxMcs][thisMcs] / 100;   // digit: 100
        thisUtility = beaconRider_getUtility(sc, maxMcs, thisMcs, thisSfer); 

        if (bestUtility < thisUtility) 
        { 
            bestUtility = thisUtility; 
            bestMcs = thisMcs; 
        } 
    } 

    // 0: No ridable MCS-> giveUp count
    // 1: ridable MCS
    if (bestUtility <= 0){
        return 0;
    }
    else return 1;
}



int beaconRider_pickRidingMcs(struct ath_softc_tgt *sc)
{
    //printk("beaconRider_pickRidingMcs\n");

    struct ath_hal *ah = sc->sc_ah;

    int i;      // iterator for MCS index
    int thisMcs, bestMcs, maxRix, minRix, maxMcs, minMcs;
    int thisUtility, bestUtility;
    int thisRateKbps, thisSfer;
    int rideeIndex = beaconRider_findBeaconIndex(sc, ah->rideeAddress);

    maxRix = ah->chosenRix;         // Selected Rix from rate adaptation algorithm
    maxMcs = beaconRider_rixToMcs(maxRix);
    if (maxMcs == -1)
    {
        // if channel quality is too bad, -1 is returned to maxMcs
        maxMcs = 0;
    }
    //minMcs = 0;
    
    minMcs = 3;
    thisUtility = 0;
    bestUtility = -9999999999;
    bestMcs = minMcs;
    
    for (i = maxMcs; i >= minMcs; i--)
    {
        thisMcs = i;
        thisRateKbps = beaconRider_mcsToKbps(thisMcs);
        thisSfer = ah->sferTable[rideeIndex].sfer[maxMcs][thisMcs] / 100;
        thisUtility = beaconRider_getUtility(sc, maxMcs, thisMcs, thisSfer);
    
        if (bestUtility < thisUtility)
        {
            bestUtility = thisUtility;
            bestMcs = thisMcs;
        }
    }

    // Probing for one level higher Rix
    if((ah->sferTable[rideeIndex].lastChosenRix == maxRix) && 
            (ah->sferTable[rideeIndex].lastRidingRix == beaconRider_mcsToRix(bestMcs)) &&
            ah->sferTable[rideeIndex].isFirstAfterGiveUp != 1)
    {
        if (ah->sferTable[rideeIndex].probeCount == 0){
            if (bestMcs < maxMcs)
                bestMcs++;        
            
            ah->sferTable[rideeIndex].isProbe = 1;
            ah->sferTable[rideeIndex].numProbingSum++;
        }else{
            ah->sferTable[rideeIndex].probeCount--;
        }    
    }
    else
    {
        ah->sferTable[rideeIndex].probeCount = ah->sferTable[rideeIndex].probeWindow;
    }

    // Static riding MCS
    // if ah->rMc  0~7, static riding MCS is used
    if (ah->rMcs >= 0 && ah->rMcs <= 7)
        bestMcs = ah->rMcs;

    ah->sferTable[rideeIndex].lastChosenRix = ah->chosenRix;
    ah->sferTable[rideeIndex].lastRidingRix = beaconRider_mcsToRix(bestMcs);

    return beaconRider_mcsToRix(bestMcs);
}

/*
   int beaconRider_getNextLowerRix(int Rix)
   {
   if (Rix == 12) return Rix;  // Lowest Rix: 6 Mbps 
   if (Rix == 23 || Rix == 21 || Rix == 19 || Rix == 17)
        return Rix - 2;
    else 
        return Rix - 1;
}

int beaconRider_getLowerRix(int Rix, int Level)
{
    int i;

    for (i = 0; i < Level; i++)
    {
        if (Rix == 12) continue; // Lowest Rix: 6 Mbps 
        if (Rix == 23 || Rix == 21 || Rix == 19 || Rix == 17)
            Rix -= 2;
        else 
            Rix--;
    }
    return Rix;
}
*/

int beaconRider_getUtility(struct ath_softc_tgt *sc, int maxMcs, int thisMcs, int thisSfer)
{
    struct ath_hal *ah = sc->sc_ah;
    
    unsigned int maxDataRate, dataRate;
    maxDataRate = beaconRider_mcsToKbps(maxMcs) / 100;
    dataRate = beaconRider_mcsToKbps(thisMcs) / 100;

    int rideeIndex = beaconRider_findBeaconIndex(sc, ah->rideeAddress);
    int beaconLength = ah->beaconList[rideeIndex].length;
    int receivedLength = ah->receiveLength;   
 
    int schedulingDelay = 96;  // Scheduling riding packet delay (77us) + configuration radio (19us)
    int subframeDelay = 3;     // Additional scheduling delay for each subframe  
    int subframeLength = 1556;
    int leftDurationMargin = 10; // Guard margin for delay offset
    int padLength = 40;
    int t_PLCP_preamble = 32; // legacy (16) + 11n(16) = 32 us 
    int t_PLCP_header = 4;  // 4 * numStreams 
    int t_PLCP_PHY = t_PLCP_preamble + t_PLCP_header;  
    //int t_backoff = 68;    // average backoff delay:  9 * 15 / 2 ;

    int numPad = 0;
    int numSubframe = 0;
    int ridingUtility = 0;
    int normalUtility = 0;
    int leftDuration= 0;
    int ridingDuration = 0;

    for (numSubframe = 1; numSubframe < 10; numSubframe++)
    {
        leftDuration = (beaconLength - receivedLength) * 8 - schedulingDelay - numSubframe * subframeDelay + leftDurationMargin;
        ridingDuration = t_PLCP_PHY + subframeLength * 8 * 10 / dataRate +
            (numSubframe - 1) * (subframeLength + padLength) * 8 * 10 / dataRate;
        if (ridingDuration >= leftDuration)
            break;
    }
   
    ridingUtility = dataRate * (100 - thisSfer) / 100 * ridingDuration;
    normalUtility = maxDataRate * (ridingDuration - leftDuration);

    return ridingUtility - normalUtility;
}


int beaconRider_calAggrLimit(struct ath_softc_tgt *sc, unsigned char *bssid, int ridingRix)
{
    struct ath_hal *ah = sc->sc_ah;
    const HAL_RATE_TABLE *rt;
    rt = sc->sc_currates;
    
    int dataRate = rt->info[ridingRix].rateKbps / 100;      // Rix doesnt matches with MCS
    int beaconLength = beaconRider_getBeaconLength(sc, bssid);
    int receivedLength = ah->receiveLength;    

    int schedulingDelay = 96;  // Scheduling riding packet delay (77us) + configuration radio (19us)
    int subframeDelay = 3;     // Additional scheduling delay for each subframe  
    int subframeLength = 1556;
    int leftDurationMargin = 10; // Guard margin for delay offset
    int padLength = 40;
    int t_PLCP_preamble = 32; // legacy (16) + 11n(16) = 32 us 
    int t_PLCP_header = 4;  // 4 * numStreams 
    int t_PLCP_PHY = t_PLCP_preamble + t_PLCP_header;  
    //int t_backoff = 68;    // average backoff delay:  9 * 15 / 2 ;

    int numPad = 0;
    int numSubframe = 0;
    int leftDuration= 0;
    int ridingDuration = 0;

    for (numSubframe = 1; numSubframe < 10; numSubframe++)
    {
        leftDuration = (beaconLength - receivedLength) * 8 - schedulingDelay - numSubframe * subframeDelay + leftDurationMargin;
        ridingDuration = t_PLCP_PHY + subframeLength * 8 * 10 / dataRate +
            (numSubframe - 1) * (subframeLength + padLength) * 8 * 10 / dataRate;
        if (ridingDuration >= leftDuration)
            break;
    }

    // hjlee: for short riding packet test
     numSubframe = 13;

    numPad = numSubframe - 1;

    return numSubframe * subframeLength + numPad * padLength + 50;  // aggr_limit margin: 50
}


int beaconRider_isRideeBeacon(struct ath_softc_tgt *sc)
{

    struct ath_hal *ah = sc->sc_ah;
    struct ath_rx_desc *ds;
    volatile unsigned char *buff;                 

    // Parse receive buffer pointer 
    ds = asf_tailq_first(&sc->sc_rxdesc);
    buff = (volatile unsigned char *)ds->ds_data;

    int i;
    struct ath_txq *txq;
    txq = ATH_TXQ(sc, 0);

    if (ah->numRxCount != 0 && ah->numRxCount == ah->tempRxCount)
    {
        // Buffer check point
        if (txq->axq_depth != 0 && buff[21] != 0xF1 && buff[0] == 0x80 && A_MEMCMP(ah->sourceAddress, buff+10, 6) == 0 )
        {  
            //printk("Ridee beacon is detected!\n");
            ah->ridingCheckCount++;
            
            for (i = 0; i < 6; i++)
            {
                ah->rideeAddress[i] = buff[16+i];
            }

            int beaconIndex = beaconRider_findBeaconIndex(sc, ah->rideeAddress);

            if (beaconIndex == -1)
            {
                //ah->ridingGiveUpCountW++;
                //printk("Wrong Beacon Address\n");
                //beaconRider_printAddr(buff);
            }
            // Only when rideeBeacon is already enlisted, riding packet can be transmitted
            else
            {
                int j;
                struct ath_tx_buf *bf;          // Arbitary packet buffer
                bf = asf_tailq_first(&txq->axq_q);

                if (ah->sferTable[beaconIndex].giveUpCount > 0)
                {
                    printk("Giving Up Riding! [");
                    printk(itos(ah->sferTable[beaconIndex].giveUpCount));
                    printk("]\n");
                    ah->sferTable[beaconIndex].giveUpCount--;
                    ah->ridingGiveUpCount++;
                    
                    // sfer compensation for lowest MCS
                    if (ah->sferTable[beaconIndex].giveUpCount == 0){
                          ah->sferTable[beaconIndex].isFirstAfterGiveUp = 1;
                    }
                    // Abort Rx
                    *((a_uint32_t *)(WLAN_BASE_ADDRESS + AR_DIAG_SW)) |= AR_DIAG_RX_ABORT;

                    // Re-enable Rx for once packet is transmitted
                    iowrite32_mac(AR_DIAG_SW, ioread32_mac(AR_DIAG_SW) & ~AR_DIAG_RX_ABORT);
                }
                else
                {
                    ah->isTxEnabled = 1;   // Ready to ride
                    
                    while (txq->axq_depth >= 1)
                    {
                        beaconRider_owltgt_tx_processq(sc, txq, OWL_TXQ_ACTIVE);
                    }
                    
                    ah->ah_setRxDP(ah, (unsigned int)ds);
                    // Abort Rx
                    *((a_uint32_t *)(WLAN_BASE_ADDRESS + AR_DIAG_SW)) |= AR_DIAG_RX_ABORT;

                }
            }
        }
    }
    return 0;
}

int beaconRider_txPacket(struct ath_softc_tgt *sc)
{
    struct ath_hal *ah = sc->sc_ah;
    struct ath_rx_desc *ds;

    // RSSI information update
    ds = asf_tailq_first(&sc->sc_rxdesc);
    struct ar5416_desc ads;
    struct ar5416_desc *adsp = AR5416DESC(ds);
    ads.u.rx = adsp->u.rx;
    ah->receiveLength = ads.ds_rxstatus1 & AR_DataLen;
    struct ath_txq *txq;
    txq = ATH_TXQ(sc, 0);
    
    ah->isRidingPacket = 1;
    
    ath_tgt_txq_schedule_wrapper(sc, txq);
     
    // Re-enable Rx for once packet is transmitted
    iowrite32_mac(AR_DIAG_SW, ioread32_mac(AR_DIAG_SW) & ~AR_DIAG_RX_ABORT);

    // Assure Rx is still enabled
    iowrite32_mac(AR_DIAG_SW, ioread32_mac(AR_DIAG_SW) & ~AR_DIAG_RX_DIS);
    iowrite32_mac(AR_CR, AR_CR_RXE);

//    OS_DELAY(20);

    beaconRider_confRadio(sc);
    //*((a_uint32_t *)(WLAN_BASE_ADDRESS + AR_Q_TXE)) = 1 << TXQUEUE;

    // This printk is required to make a delay before transmision
    beaconRider_printTime();
    ah->lastRidingTime = NOW()/117;
    printk("BeaconRider: RIDING PACKET IS TRANSMITTED\n");
    ah->txRidingPacketCount++;
    ah->isTxEnabled = 0;

    /*
    // Re-enable Rx for once packet is transmitted
    iowrite32_mac(AR_DIAG_SW, ioread32_mac(AR_DIAG_SW) & ~AR_DIAG_RX_ABORT);

    // Assure Rx is still enabled
    iowrite32_mac(AR_DIAG_SW, ioread32_mac(AR_DIAG_SW) & ~AR_DIAG_RX_DIS);
    iowrite32_mac(AR_CR, AR_CR_RXE);
    */
    // Restore radio configuration
    beaconRider_restoreRadio(sc);
    // re-enable interrupts
    //ah->ah_setInterrupts(sc->sc_ah, sc->sc_imask);

    return 0;
}

