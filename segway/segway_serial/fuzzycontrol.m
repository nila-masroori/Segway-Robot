function y = fuzzycontrol(fis,err_dot,err)
y=-evalfis([err,err_dot],fis)
end