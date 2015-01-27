$BIN_PATH = "./build"
$INSTANCES_PATH = "/home/paul/Cloud/studium/Cluster/programs/instances"
$NAMES = ["ce-approx2k"]
#$NAMES = ["ce-approx3", "ce-approx2k"]
$TYPE = "synthetic"

puts `cd #{$BIN_PATH} && make`
diff = Hash.new
counter = 0
entries = Dir.entries($INSTANCES_PATH+"/"+$TYPE)
entries.each do |graph|
    next if graph == "k.txt" or graph == "k-cvd.txt" or graph.end_with? ".zip" 
    next if graph.start_with? "."
    next if not graph.end_with? ".txt" or graph.end_with? ".graph"
    grep = `grep #{graph} #{$INSTANCES_PATH}/#{$TYPE}/k.txt`
    kcorrect = grep.split()[1].to_f
    puts graph
    $NAMES.each do |prog|
        k = `#{$BIN_PATH}/#{prog} < "#{$INSTANCES_PATH}/#{$TYPE}/#{graph}" | grep -ve "^#" | grep -v '^$' | wc -l`.to_f
        puts "#{k} #{kcorrect}"
        if(diff[prog].nil?)
            diff[prog] = 0.0
        end
        if(kcorrect < 0.1)
            diff[prog] += (k+0.1)/(kcorrect+0.1)
        else
            diff[prog] += k / kcorrect
        end
    end
    counter += 1
end
diff.each do |key,val|
    puts "#{key}:#{val/counter}"
end

=begin
for GRAPH in ${FILES[@]}; do
    g=$(basename $GRAPH)
    if [[ "$g" == "k.txt" ]]; then
        continue
    fi
    if [[ "$g" == "k-cvd.txt" ]]; then
        continue
    fi

    kfull=$(grep $g $INSTANCES_PATH/$TYPE/k.txt)
    kfull2=($kfull)
    kcorrect=${kfull2[1]}


    for IX in ${!names[*]}; do
        PROG=${names[$IX]}
        kb=$($BIN_PATH/$PROG < "$GRAPH" | grep -ve "^#" | grep -v '^$' | wc -l)
        echo "$TYPE;$kcorrect;$kb;$TYPE/$g;$PROG"
    done
done
=end
