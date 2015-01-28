def parse(a)
    b = a.split(";")
    val = b[0].to_f
    opt = b[1].to_i
    k = b[2].to_i
    graph = b[3]
    prog = b[4]
    type = b[5]
    return val,opt,k,graph,prog,type
end
while a = gets 
    next if a.start_with?('#')
    val,opt,k,graph,prog,type = parse(a)
    if(val > 1.03)
        if(Random.rand(3) == 0)
            puts "#{graph} #{opt}"
        end
    else
        if(Random.rand(12) == 0)
            puts "#{graph} #{opt}"
        end 
    end
end

