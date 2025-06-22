[flute, fs] = audioread("flute.wav");

%% Infinite Clip
flute_infiniteClip = InfiniteClipping(flute);
soundsc(flute_infiniteClip, fs);

%% Half Wave Rectification

flute_rectification = HalfWaveRectification(flute);
soundsc(flute_rectification,fs);

%% Full Wave Rectification

flute_full_Rectification = FullWaveRectification(flute);
soundsc(flute_full_Rectification, fs);

%% Hard Clip

flute_HardClip = HardClip(flute, 0.25);
soundsc(flute_HardClip, fs);

%% Soft Clip

flute_SoftClip = SoftClip(flute, 0.25, "Arctan");
soundsc(flute_SoftClip, fs);

%% PieceWise Overdrive
flute_pwo = PieceWiseOverdrive(flute);
soundsc(flute_pwo,fs);

%% Diode Clipping
flute_diodeClip = DiodeClipping(flute);
soundsc(flute_diodeClip,fs);

%% Bit Crusher
flute_bitCrush = BitCrusher(flute, 8);
soundsc(flute_bitCrush,fs);