/*
 * smf_callback.h
 *
 *  Created on: 2016/10/26
 *      Author: hiromichihatano
 */

#ifndef SMF_CALLBACK_H_
#define SMF_CALLBACK_H_

#include "smf_base.h"
#include "smf_lib.h"

/**
 * @brief Midi Event 発生時に呼び出されるコールバック関数
 *
 * Midi 各イベント発生時に呼び出されるコールバック関数。
 * 必要なコールバック関数のみ登録し、不要な場合は NULL とすること。
 *
 * @note 時間のかかる処理を行うと演奏処理が遅延するため、これらの関数内で行うべきでない。
 */
typedef struct smf_callback {
    /**
     * @brief Note Off イベントに対応するコールバック関数
     * 
     * @param channel イベント発生チャネル
     * @param key キー番号
     * @param velocity velocity 値
     */
    void (*noteOff)(uint8_t channel, uint8_t key, uint8_t velocity);

    /**
     * @brief Note On イベントに対応するコールバック関数
     * 
     * @param channel イベント発生チャネル
     * @param key キー番号
     * @param velocity velocity 値
     */
    void (*noteOn)(uint8_t channel, uint8_t key, uint8_t velocity);

    /**
     * @brief Poly Key Pressure イベントに対応するコールバック関数
     * 
     * @param channel イベント発生チャネル
     * @param key キー番号
     * @param velocity velocity 値
     */
    void (*polyKeyPressure)(uint8_t channel, uint8_t key, uint8_t velocity);

    /**
     * @brief Control Change イベントに対応するコールバック関数
     * 
     * @param channel イベント発生チャネル
     * @param ctrlNum コントロールチェンジ番号
     * @param value コントロールチェンジデータ
     */
    void (*controlChange)(uint8_t channel, uint8_t ctrlNum, uint8_t value);

    /**
     * @brief Program Change イベントに対応するコールバック関数
     * 
     * @param channel イベント発生チャネル
     * @param ctrlNum プログラム番号
     */
    void (*programChange)(uint8_t channel, uint8_t programNum);

    /**
     * @brief Channel Pressure イベントに対応するコールバック関数
     * 
     * @param channel イベント発生チャネル
     * @param pressureValue Pressure 値
     */
    void (*channelPressure)(uint8_t channel, uint8_t pressureValue);

    /**
     * @brief Pitch Bend イベントに対応するコールバック関数
     * 
     * @param channel イベント発生チャネル
     * @param bendValue Pitch bend 値
     */
    void (*pitchBend)(uint8_t channel, int32_t bendValue);

    /**
     * @brief System Exclusive メッセージに対応するコールバック関数
     * 
     * @param buf MIDI バッファ配列 (buf[beginOff] ～ buf[beginOff + sysExLen - 1])
     * @param beginOff buf 内メッセージ開始オフセット(buf[beginOff] で先頭要素にアクセス)
     * @param sysExLen メッセージ長
     * @param sysExType 0xF0 or 0xF7(F7 はまれ)
     * 
     * @note 現在上位では SysEx を全くデコードしない。Type, subID 等は関数内でデコードすること
     */
    void (*sysEx)(const uint8_t buf[], off_t beginOff, len_t sysExLen, uint8_t sysExType);

    /**
     * @brief Meta イベントに対応するコールバック関数
     * 
     * @param buf MIDI バッファ配列 (buf[beginOff] ～ buf[beginOff + metaEventLen - 1])
     * @param beginOff buf 内イベント開始オフセット(buf[beginOff] で先頭要素にアクセス)
     * @param metaEventLen メッセージ長
     * @param metaEventType メタイベントタイプ
     * 
     * @note 現在、テンポイベントのみ別で処理される。それ以外はこの関数で行う
     */
    void (*metaEvent)(const uint8_t buf[], off_t beginOff, len_t metaEventLen, uint8_t metaEventType);

    /**
     * @brief テンポセット Meta イベントに対応するコールバック関数
     * 
     * @param tempo テンポ値。
     * 
     * @note SMF interpreter 内部のテンポ値はこの関数で書き換える必要はない
     */
    void (*metaEventTempo)(int32_t tempo);
} smf_callback_t;

#endif /* SMF_CALLBACK_H_ */