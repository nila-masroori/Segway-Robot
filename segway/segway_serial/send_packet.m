function res = send_packet(ss , left_motor , right_motor)
    L = uint8(typecast(int16(left_motor), 'uint8'));
    R = uint8(typecast(int16(right_motor), 'uint8'));
    tmp_vec = uint8([0, 0, L, R, 255]);
    fwrite(ss , tmp_vec , 'uint8','async');
    flushinput(ss)
    res = tmp_vec;
end