% Copyright(C) 2018 The University of Texas at Dallas
% Developed By: Muhammet Emin Yanik
% Advisor: Prof. Murat Torlak
% Department of Electrical and Computer Engineering
% 
% This work was supported by the Semiconductor Research Corporation (SRC) task 2712.029
% through The University of Texas at Dallas' Texas Analog Center of Excellence (TxACE).
% 
% Redistributions and use of source must retain the above copyright notice
% Redistributions in binary form must reproduce the above copyright notice
% 
% 
% Module Name:
% SarSyncApi.m
% 
% Abstract:
% The Api class to communicate with the hardware


classdef SarSyncApi
    
    %% Properties
    properties
        serialPort   % Serial port object
    end
    
    %% Methods
    methods
        %% Constructor
        function obj = SarSyncApi(port)
            if nargin == 1
                obj.serialPort = serialport(port,115200);
                configureTerminator(obj.serialPort,"CR/LF")
                % configureCallback(obj.serialPort,"terminator",@obj.readSerialData)
                flush(obj.serialPort)
            end
        end
        
        %% Radar Trigger Command
        function radarTrigger(obj)
            write(obj.serialPort,"$RTG#","char")
        end
        
        %% Set Pulse Count Command
        function setPulseCount(obj, numPulses)
            write(obj.serialPort, "$PLS" + num2str(numPulses) + "#", "char")
        end
        
        %% Reset Pulse Counter Command
        function resetPcnt(obj)
            write(obj.serialPort,"$RST#","char")
        end
        
        %% Set Number of Measurement Command
        function setNumMeasuremen(obj, numMeasurement)
            write(obj.serialPort, "$MSR" + num2str(numMeasurement) + "#", "char")
        end
    end
end