#=== Deal with inputs.
if ARGV.length < 5
	puts "ce_wrapper.rb is a wrapper for the Cluster editing algorithm."
	puts "Usage: ruby saps_wrapper.rb <instance_relname> <instance_specifics> <cutoff_time> <cutoff_length> <seed> <params to be passed on>."
	exit -1
end

cnf_filename = ARGV[0]
instance_specifics = ARGV[1].to_f
cutoff_time = ARGV[2].to_f
cutoff_length = ARGV[3].to_i
seed = ARGV[4].to_i

paramstring = ARGV[5...ARGV.length].join(" ")
start = Time.now
k = `timeout #{cutoff_time}s ../../build/ce-approx2k #{paramstring} < #{cnf_filename}  | grep -ve "^#" | grep -v '^$' | wc -l`.to_f
finish = Time.now

solved = "SUCCESS"
runtime = finish - start
runlength = 1
best_sol = 2
kcorrect = instance_specifics
if(k < 0.1 and kcorrect > 0.1)
    solved = "TIMEOUT"
else
    if(kcorrect < 0.1)
        best_sol = (k+0.1)/(kcorrect+0.1)
    else
        best_sol = k / kcorrect
    end
end
puts "Result of algorithm run: #{solved}, #{runtime}, #{runlength}, #{best_sol}, #{seed}"
