/*
 * beatmap.h
 *
 *  Created on: 08/dic/2011
 *      Author: vittorio
 */

#pragma once

#include <QMutex>

#include "proto/beats.pb.h"
#include "track/beats.h"

#define BEAT_MAP_VERSION "BeatMap-1.0"

class Track;

typedef QList<mixxx::track::io::Beat> BeatList;

namespace mixxx {

class BeatMap final : public Beats {
  public:
    // Construct a BeatMap. iSampleRate may be provided if a more accurate
    // sample rate is known than the one associated with the Track.
    BeatMap(const Track& track, SINT iSampleRate);

    BeatMap(const Track& track,
            SINT sampleRate,
            const QString& subVersion,
            BeatList beats,
            double nominalBpm);

    ~BeatMap() override = default;

    // Construct a BeatMap. iSampleRate may be provided if a more accurate
    // sample rate is known than the one associated with the Track. If it is
    // zero then the track's sample rate will be used. The BeatMap will be
    // deserialized from the byte array.
    static BeatsPointer makeBeatMap(const Track& track,
            SINT sampleRate,
            const QString& subVersion,
            const QByteArray& byteArray);

    // Construct a BeatMap. iSampleRate may be provided if a more accurate
    // sample rate is known than the one associated with the Track. If it is
    // zero then the track's sample rate will be used. A list of beat locations
    // in audio frames may be provided.
    static BeatsPointer makeBeatMap(const Track& track,
            SINT sampleRate,
            const QString& subVersion,
            const QVector<double>& beats);

    Beats::CapabilitiesFlags getCapabilities() const override {
        return BEATSCAP_TRANSLATE | BEATSCAP_SCALE | BEATSCAP_ADDREMOVE |
                BEATSCAP_MOVEBEAT;
    }

    QByteArray toByteArray() const override;
    QString getVersion() const override;
    QString getSubVersion() const override;
    void setSubVersion(const QString& subVersion) override;

    ////////////////////////////////////////////////////////////////////////////
    // Beat calculations
    ////////////////////////////////////////////////////////////////////////////

    double findNextBeat(double dSamples) const override;
    double findPrevBeat(double dSamples) const override;
    bool findPrevNextBeats(double dSamples,
                           double* dpPrevBeatSamples,
                           double* dpNextBeatSamples) const override;
    double findClosestBeat(double dSamples) const override;
    double findNthBeat(double dSamples, int n) const override;
    std::unique_ptr<BeatIterator> findBeats(double startSample, double stopSample) const override;
    bool hasBeatInRange(double startSample, double stopSample) const override;

    double getBpm() const override;
    double getBpmAroundPosition(double curSample, int n) const override;

    SINT getSampleRate() const override {
        return m_iSampleRate;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Beat mutations
    ////////////////////////////////////////////////////////////////////////////

    BeatsPointer clone() const override;
    BeatsPointer translate(double dNumSamples) const override;
    BeatsPointer scale(enum BPMScale scale) const override;
    BeatsPointer setBpm(double dBpm) override;

  private:
    BeatMap(const BeatMap& other);
    // Constructor to update the beat grid
    BeatMap(const BeatMap& other, BeatList beats, double nominalBpm);

    // For internal use only.
    bool isValid() const;

    mutable QMutex m_mutex;
    QString m_subVersion;
    SINT m_iSampleRate;
    double m_nominalBpm;
    BeatList m_beats;
};

} // namespace mixxx
