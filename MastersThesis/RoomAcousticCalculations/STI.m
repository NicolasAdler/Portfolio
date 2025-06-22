function [STI_val, ALcons] = STI(ImpulseResponse, fs, OctaveFilters)

    centerFreqs = [125, 250, 500, 1000, 2000, 4000, 8000];
    centerFreqs(centerFreqs > fs / 2) = []; % Limit to Nyquist frequency
    numBands = numel(centerFreqs);

    if nargin < 3
        OctaveFilters = cell(1, numBands);
        for i = 1:numBands
            OctaveFilters{i} = octaveFilter('CenterFrequency', centerFreqs(i), ...
                                            'Bandwidth', '1/3 octave', ...
                                            'SampleRate', fs, ...
                                            'FilterOrder', 6);
        end
    end

    getMTF = @(ir) abs(fft(ir .^ 2) / sum(ir .^ 2));

    modulationFreqs = 2 .^ ((-2:11) / 3);
    MTF_octband = zeros(length(modulationFreqs), numBands);
    for i = 1:numBands

    ir_filtered = OctaveFilters{i}(ImpulseResponse);

    MTF = getMTF(ir_filtered);

    MTF_half = MTF(1:floor(end/2) + 1);

    MTF_octband(:, i) = interp1(linspace(0, fs/2, numel(MTF_half)), ...
                                MTF_half, ...
                                modulationFreqs, ...
                                'linear', NaN);
    end

    validBands = ~any(isnan(MTF_octband), 1);
    MTF_octband(:, ~validBands) = [];
    centerFreqs(~validBands) = [];

    SNR_apparent = pow2db(MTF_octband ./ (1 - MTF_octband));

    SNR_apparent = max(min(SNR_apparent, 15), -15);

    weights = [0.13, 0.14, 0.11, 0.12, 0.19, 0.17, 0.14];
    weights = weights(validBands) / sum(weights(validBands));
    SNR_weighted = sum(mean(SNR_apparent, 1) .* weights);

    STI_val = (SNR_weighted + 15) / 30;

    ALcons = 170.5405 * exp(-5.419 * STI_val);

end
