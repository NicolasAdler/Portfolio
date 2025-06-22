% CONVERTSECSAMPLES
% This script provides two examples for converting a time delay
% in units of seconds to samples and milliseconds to samples
%
% See also CONVERTTEMPOSAMPLES
% Example 1 – Seconds to Samples
Fs = 48000; % arbitrary sampling rate
timeSec = 1.5; % arbitrary time in units of seconds
% Convert to units of samples
timeSamples = fix(timeSec * Fs); % round to nearest integer sample
% Example 2 – Milliseconds to Samples
timeMS = 330; % arbitrary time in units of milliseconds
% Convert to units of seconds
timeSec = timeMS/1000;
% Convert to units of samples
timeSamples = fix(timeSec * Fs); % round to nearest integer sample
%% CONVERTTEMPOSAMPLES
% This script provides an example for calculating a delay time
% in units of samples that will be synchronized with the tempo
% of a song in units of beats per minutes (BPM).
%
% Assume a (4/4) time signature where a BEAT = QUARTER NOTE
%
% See also CONVERTSECSAMPLES
% Example – Convert Tempo Sync'd Delay to Samples
Fs = 48000; % arbitrary sampling rate
beatsPerMin = 90; % arbitrary tempo in units of beats per minute
% Calculate beats per second
beatsPerSec = beatsPerMin / 60; % 1 minute / 60 seconds
% Calculate # of seconds per beat
secPerBeat = 1/beatsPerSec;
% Note division
% 4 = whole, 2 = half, 1 = quarter, 0.5 = 8th, 0.25 = 16th
noteDiv = 1 ;
% Calculate delay time in seconds
timeSec = noteDiv * secPerBeat;
% Convert to units of samples
timeSamples = fix(timeSec * Fs); % round to nearest integer sample

