import numpy
import subprocess
import Queue
import sys

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

def calculate_coefficient (exponent_list, length, missing_index):
	to_return = 'SeriesCoefficient[1'
	degree_missing = calculate_degree (exponent_list[missing_index])
	i = 0
	while i < length:
		if i == missing_index:
			i += 1
			continue
		to_return += "(1+"
		of_interest = exponent_list[i]
		degree = calculate_degree (of_interest)
		power = degree_missing/degree + 1
		j = 1
		while j <= power:
			to_return += create_monomial (symbols, of_interest*j)
			to_return += "+"
			j += 1
		to_return = to_return[:-1]
		to_return += ")"
		i += 1
	to_return += ","
	i = 0
	while i < dimension:
		to_return += "{"+symbols[i]+",0,"+str(exponent_list[missing_index][i])+"},"
		i += 1
	to_return = to_return[:-1]
	to_return += "]"
	#print >> sys.stderr, to_return
	p = subprocess.Popen(['/usr/bin/runMath', to_return], stdin=subprocess.PIPE, stdout=subprocess.PIPE, bufsize=1)
	#out, err = p.communicate()
	out = p.stdout.readline()
	#print >> sys.stderr, out
	try:
		int_value = int (out[:-1])
		return int_value
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
	old_gen = gns.generators
	missing_generator = old_gen[missing_index]
	degree_missing_gen = calculate_degree (missing_generator)
	if degree_missing_gen == 1:
		to_return = numpy.zeros((2*num_generators, dimension), dtype=numpy.int)
		to_return[-1] = missing_generator*3
	else:
		to_return = numpy.zeros((2*num_generators-1, dimension), dtype=numpy.int)
	index = 0
	for i in range(num_generators):
		of_interest = old_gen[i]
		if i != missing_index:
			to_return[index] = old_gen[i]
			index += 1
		to_return[num_generators-1+i] = of_interest+missing_generator
	return to_return

def get_min_gen (potential_generators, where_start_new):
	num_gens = len(potential_generators)
	to_return = numpy.zeros((num_gens, dimension),dtype=numpy.int)
	to_return[:where_start_new, :] = potential_generators[:where_start_new, :]
	index = where_start_new
	for i in range(where_start_new,num_gens):
		while True:
			coefficient = calculate_coefficient (potential_generators, num_gens, i)
			if coefficient != None:
				break
		if coefficient == 0:
			to_return[index] = potential_generators[i]
			index += 1
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
		min_gens = get_min_gen (potential, num_generators-1)
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
		




