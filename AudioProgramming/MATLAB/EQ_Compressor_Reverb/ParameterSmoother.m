classdef ParameterSmoother < matlab.System
    properties
        smoothingFactor = 0.99; % Smoothing factor (0 < smoothingFactor < 1)
    end
    
    properties (Access = private)
        currentValue
        targetValue
    end
    
    methods
        function obj = ParameterSmoother(initialValue, smoothingFactor)
            if nargin > 0
                obj.currentValue = initialValue;
                obj.targetValue = initialValue;
                obj.smoothingFactor = smoothingFactor;
            end
        end
        
        function setTargetValue(obj, value)
            obj.targetValue = value;
        end
    end
    methods(Access = protected)
        function smoothedValue = stepImpl(obj)
            % Perform the smoothing
            obj.currentValue = obj.smoothingFactor * obj.currentValue + ...
                               (1 - obj.smoothingFactor) * obj.targetValue;
            smoothedValue = obj.currentValue;
        end
    end
end
