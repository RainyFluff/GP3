#include "GP3TriggerShake.h"

UGP3TriggerShake::UGP3TriggerShake()
{
    OscillationDuration = 1.0f;
    OscillationBlendInTime = 0.1f;
    OscillationBlendOutTime = 0.2f;
    bSingleInstance = false;

    RotOscillation.Pitch.Amplitude = 2.5f;
    RotOscillation.Pitch.Frequency = 10.0f;
    RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
    RotOscillation.Pitch.Waveform = EOscillatorWaveform::SineWave;

    RotOscillation.Yaw.Amplitude = 2.5f;
    RotOscillation.Yaw.Frequency = 10.0f;
    RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
    RotOscillation.Yaw.Waveform = EOscillatorWaveform::SineWave;

    RotOscillation.Roll.Amplitude = 0.0f;
    RotOscillation.Roll.Frequency = 0.0f;
    RotOscillation.Roll.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
    RotOscillation.Roll.Waveform = EOscillatorWaveform::SineWave;

    LocOscillation.X.Amplitude = 50.0f;
    LocOscillation.X.Frequency = 1.0f;
    LocOscillation.X.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
    LocOscillation.X.Waveform = EOscillatorWaveform::SineWave;

    LocOscillation.Y.Amplitude = 0.0f;
    LocOscillation.Y.Frequency = 0.0f;
    LocOscillation.Y.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
    LocOscillation.Y.Waveform = EOscillatorWaveform::SineWave;

    LocOscillation.Z.Amplitude = 10.0f;
    LocOscillation.Z.Frequency = 25.0f;
    LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
    LocOscillation.Z.Waveform = EOscillatorWaveform::SineWave;
}

