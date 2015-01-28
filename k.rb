$BIN_PATH = "./build"
$INSTANCES_PATH = "smac/instances"
$NAMES = ["ce-approx2k -2k_diff 2", "ce-approx2k -2k_diff 1.8", "ce-approx2k -2k_diff 1.5"]
def getType(name)
    if(name.include? "optK") 
        return "BIO2"
    end
    if(name.include? "nr") 
        return "BIO"
    end
    return "SYN"
end
`cd #{$BIN_PATH} && make`
diff = Hash.new
correct = Hash.new
counter = 0
entries = Dir.entries($INSTANCES_PATH+"/")
entries.each do |graph|
    next if graph == "k.txt" or graph == "k-cvd.txt" or graph.end_with? ".zip" 
    next if graph.start_with? "."
    next if not (graph.end_with? ".txt" or graph.end_with? ".graph")
    grep = `grep #{graph} #{$INSTANCES_PATH}/all.k`
    kcorrect = grep.split()[1].to_f
    $NAMES.each do |prog|
        k = `#{$BIN_PATH}/#{prog} < "#{$INSTANCES_PATH}/#{graph}" | grep -ve "^#" | grep -v '^$' | wc -l`.to_f
        if(diff[prog].nil?)
            diff[prog] = 0.0
            correct[prog] = 0
        end
        if(kcorrect == k)
            correct[prog] += 1
        end
        val = 0.0
        if(kcorrect < 0.1)
            val = (k+0.1)/(kcorrect+0.1)
        else
            val = k / kcorrect
        end
        diff[prog] += val
        puts "#{val};#{kcorrect};#{k};#{graph};#{prog};#{getType(graph)};#{counter}"
    end
    counter += 1
end
diff.each do |key,val|
    puts "#size: #{key} #{val/counter}"
    puts "#solved: #{key} #{correct[key]}"
    puts "#solved percent: #{key} #{correct[key].to_f/counter.to_f}"
end


