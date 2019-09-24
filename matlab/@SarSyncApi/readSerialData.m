function readSerialData(src,evt)
    % data = read(src,src.NumBytesAvailable,"char");
    data = readline(src);
    disp(data)
    disp(evt)
end