import numpy
import subprocess
import Queue
import sys
import copy

# there are a couple of things you need to do
# first, you need the coefficient code. you pass in



def create_symbols():
	return list('x'+str(i) for i in range(dimension))


class GNS:
	def __init__(self, generators, num_generators, index_of_effective, frobenius, hole_set, num_holes, frobenius_degree, children, parent, id):
		self.generators = generators
		self.num_generators = num_generators
		self.index_of_effective = index_of_effective
		self.frobenius = frobenius
		self.hole_set = hole_set
		self.num_holes = num_holes
		self.frobenius_degree = frobenius_degree
		self.children = children
		self.parent = parent
		self.id = id

def calculate_degree(vector):
	return sum(vector)

def create_monomial (symbols, powers):
	to_return = [symbols[i]+"^"+str(powers[i]) for i in range(dimension)]
	return "*".join(to_return)

def calculate_coefficient (exponent_list, num_old_gen, length, bound_degree):
	to_return = 'f := Expand[1'
	#print "exponent list", exponent_list
	i = 0
	while i < length:
		to_return += "(1+"
		of_interest = exponent_list[i]
		degree = calculate_degree (of_interest)
		power = bound_degree/degree + 1
		j = 1
		while j <= power:
			to_return += create_monomial (symbols, of_interest*j)
			to_return += "+"
			j += 1
		to_return = to_return[:-1]
		to_return += ")"
		i += 1
	to_return += "]; ["
	i = num_old_gen
	while i < length:
		to_return += "SeriesCoefficient[f,"
		j = 0
		while j < dimension:
			to_return += "{"+symbols[j]+",0,"+str(exponent_list[i][j])+"},"
			j += 1
		to_return = to_return[:-1]
		to_return += "],"
		i += 1
	to_return = to_return[:-1]
	to_return += "]"
	#print >> sys.stderr, to_return
	p = subprocess.Popen(['/usr/bin/runMath', to_return], stdin=subprocess.PIPE, stdout=subprocess.PIPE, bufsize=1)
	#out, err = p.communicate()
	out = p.stdout.readline()
	#print out
	#print >> sys.stderr, out
	try:
		step_one = out[5:-2]
		#print "step one", step_one
		final = [int(i) for i in out[5:-2].split(",")]
		#print "sldkfjlskfj"
		#print final
		return final
	except ValueError:
		print >> sys.stderr, to_return
		print >> sys.stderr, out
		return None

def create_genus_zero ():
	to_return = numpy.zeros((dimension, dimension), dtype=numpy.int)
	for i in range(dimension):
		to_return[i][dimension-1-i] = 1
	gns = GNS (to_return, dimension, 0, None, None, 0, 0, None, -1, 0)
	return gns

def create_potential_generators (gns, missing_index):
	num_generators = gns.num_generators
	#print >> sys.stderr, "num generators", num_generators
	old_gen = gns.generators
	#print >> sys.stderr, "generators", old_gen
	missing_generator = old_gen[missing_index]
	#print >> sys.stderr, "missing_generator", missing_generator
	#print "missing generator 1", missing_generator
	degree_missing_gen = calculate_degree (missing_generator)
	# bound should be the degree, not the element
	bound = 2*calculate_degree (missing_generator) + 2
	#bound = 2*add_one (missing_generator)
	#print "missing generator 2", missing_generator
	#print "bound is:", bound
	to_return = numpy.zeros((2*num_generators, dimension), dtype=numpy.int)
	index = 0
	index2 = 0
	#print >> sys.stderr, "num generators", num_generators
	for i in range(num_generators):
		of_interest = old_gen[i]
		#print >> sys.stderr, "i", i
		#print >> sys.stderr, "of interest", of_interest
		if i != missing_index:
			to_return[index] = old_gen[i]
			index += 1
		potential = of_interest+missing_generator
		if calculate_degree (potential) >= bound:
			#print >> sys.stderr, "missing index", missing_index
			#print >> sys.stderr, "index", index
			#print >> sys.stderr, "i", i
			to_return[index:num_generators-1] = old_gen[i+1:num_generators]
			break
		#if compare (potential, bound) != 0:
			#break
		#print "potential:", potential
		to_return[num_generators-1+index2] = potential
		index2 += 1
	if calculate_degree (3*missing_generator) < bound:
	#if compare (3*missing_generator, bound) == 0:
		to_return[num_generators-1+index2] = 3*missing_generator
		index2 += 1
	#print >> sys.stderr, "potential min gens", to_return[:num_generators-1+index2]
	return to_return[:num_generators-1+index2]

def get_min_gen (potential_generators, where_start_new, bound_degree):
	num_gens = len(potential_generators)
	to_return = numpy.zeros((num_gens, dimension),dtype=numpy.int)
	to_return[:where_start_new, :] = potential_generators[:where_start_new, :]
	coefficients = calculate_coefficient (potential_generators, where_start_new, num_gens, bound_degree)

	index = where_start_new
	for j in range (where_start_new, num_gens):
		of_interest = coefficients[j-where_start_new]
		if of_interest == 1:
			to_return[index] = potential_generators[j]
			index += 1
	#print >> sys.stderr, "min gens", to_return[:index]
	return to_return[:index]




def compare (first, second):
	# 0 means that first is smaller
	# 1 means that second is smaller
	first_deg = calculate_degree (first)
	second_deg = calculate_degree (second)
	if first_deg < second_deg:
		return 0
	if second_deg < first_deg:
		return 1
	for i in range(dimension):
		if first[i] < second[i]:
			return 0
		if second[i] < first[i]:
			return 1
	return -1

def add_one (vector):
	length = len(vector)
	copied = copy.deepcopy(vector)
	for i in range (length-1, -1, -1):
		if vector[i] != 0:
			if i != 0:
				copied[i] -= 1
				copied[i-1] += 1
				return copied
	copied = numpy.zeros(dimension, dtype=int)
	copied[length-1] = calculate_degree(vector)+1
	return copied
		

def sorted_generators (min_gens, num_old_gen, missing_generator):
	num_total = len(min_gens)
	to_return = numpy.zeros((num_total, dimension),dtype=numpy.int)
	index = 0
	old_index = 0
	new_index = num_old_gen
	effective_found = False
	index_effective = num_total-1
	while old_index < num_old_gen or new_index < num_total:
		if old_index == num_old_gen:
			added = min_gens[new_index]
			new_index += 1
		elif new_index == num_total:
			added = min_gens[old_index]
			old_index += 1
		else:
			if compare (min_gens[old_index], min_gens[new_index]) == 0:
				added = min_gens[old_index]
				old_index += 1
			else:
				added = min_gens[new_index]
				new_index += 1
		to_return[index] = added
		if effective_found == False:
			if compare (missing_generator, added) == 0:
				index_effective = index
				effective_found = True
		index += 1
	return to_return, index_effective

def print_gns (of_interest):
	print "------------------------------"
	print "Generators:"
	print of_interest.generators
	print "Number of generators:"
	print of_interest.num_generators
	print "Index of effective generators:"
	print of_interest.index_of_effective
	print "Frobenius element:"
	print of_interest.frobenius
	print "Hole set:"
	print of_interest.hole_set
	print "Number of holes:"
	print of_interest.num_holes
	print "Frobenius degree:"
	print of_interest.frobenius_degree
	print "Parent id:"
	print of_interest.parent
	print "Self id:"
	print of_interest.id
	print "------------------------------"
	return



dimension = 2
symbols = create_symbols()
queue = Queue.Queue()
total = 0
genus_zero = create_genus_zero ()
print_gns (genus_zero)
queue.put (genus_zero)
hole_number = 0
num_seen = 0
while True:
	of_interest = queue.get()
	num_holes = of_interest.num_holes
	print >> sys.stderr, num_seen	
	if num_holes > hole_number:
		print >> sys.stderr, "Finished with "+str(hole_number+1)
		print >> sys.stderr, "Number seen: "+str(queue.qsize()+1)
		hole_number += 1
		num_seen = 0
	num_seen += 1
	if num_holes == 10:
		break
	num_generators = of_interest.num_generators
	index_of_effective = of_interest.index_of_effective
	for i in range(index_of_effective, num_generators):
		missing_generator = of_interest.generators[i]
		potential = create_potential_generators (of_interest, i)
		min_gens = get_min_gen (potential, num_generators-1, 2*calculate_degree(missing_generator)+1)
		sorted, index_of_effective = sorted_generators (min_gens, num_generators-1, missing_generator)
		old_hole_set = of_interest.hole_set
		if old_hole_set != None:
			new_hole_set = numpy.append (old_hole_set, [missing_generator], axis=0)
		else:
			new_hole_set = numpy.asarray([missing_generator])
		total += 1
		new_gns = GNS (sorted, len(sorted), index_of_effective, missing_generator, new_hole_set, of_interest.num_holes+1, calculate_degree (missing_generator), None, of_interest.id, total)
		print_gns (new_gns)
		queue.put(new_gns)
		





