// This header file is made for beaconRider support
#ifndef IF_OWL_H
#define IF_OWL_H

a_uint32_t ath_pkt_duration_wrapper(struct ath_softc_tgt *sc,
				   a_uint8_t rix, struct ath_tx_buf *bf,
				   a_int32_t width, a_int32_t half_gi);

struct ath_tx_buf *ath_buf_toggle_wrapper(struct ath_softc_tgt *sc,
				      struct ath_tx_buf *bf, a_uint8_t retry);

void ath_tgt_rate_findrate_wrapper(struct ath_softc_tgt *sc,
				  struct ath_node_target *an,
				  a_int32_t shortPreamble,
				  size_t frameLen,
				  a_int32_t numTries,
				  a_int32_t numRates,
				  a_int32_t stepDnInc,
				  a_uint32_t rcflag,
				  struct ath_rc_series series[],
				  a_int32_t *isProbe);

void ath_tgt_tx_sched_normal_wrapper(struct ath_softc_tgt *sc, ath_atx_tid_t *tid);

void ath_tgt_tx_enqueue_wrapper(struct ath_txq *txq, struct ath_atx_tid *tid);

void ath_buf_comp_wrapper(struct ath_softc_tgt *sc, struct ath_tx_buf *bf);


void ath_tgt_txq_schedule_wrapper(struct ath_softc_tgt *sc, struct ath_txq *txq);


void beaconRider_owltgt_tx_processq(struct ath_softc_tgt *sc, struct ath_txq *txq,
			owl_txq_state_t txqstate);

#endif


