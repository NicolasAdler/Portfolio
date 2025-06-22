classdef ALconsCalculator
    % ALconsCalculator Class to calculate the percentage of Articulation Loss of Consonants (%ALcons)
    
    properties
        r             % Array of distances from the nearest loudspeaker
        RT60          % Reverberation Time (T60)
        num_sources   % Total number of equal sources
        volume        % Volume of the room
        Q             % Directivity of the nearest source
        M             % Acoustic modifier for reverberant power
        K             % Listener factor
        AlconsPercentValues % Array to store %ALcons values
    end
    
    methods
        function obj = ALconsCalculator(radius, RT60, number_of_equal_sources, volume, directivity_Q, acoustic_modifier_M, listener_factor_K)
            obj.r = radius;
            obj.RT60 = RT60;
            obj.num_sources = number_of_equal_sources;
            obj.volume = volume;
            obj.Q = directivity_Q;
            obj.M = acoustic_modifier_M;
            obj.K = listener_factor_K;
            obj.AlconsPercentValues = zeros(1, length(radius));
        end

        function ALconsPercentage = CalculateALconsPercentage(obj)
            for n = 1:length(obj.r)
                ALconsPercentage = ( (200 * (obj.r(n)^2) * obj.RT60 * obj.num_sources) / (obj.volume * obj.Q * obj.M) ) + obj.K;
                obj.AlconsPercentValues(n) = ALconsPercentage;
            end
            disp(obj.AlconsPercentValues)
        end
    end
end
