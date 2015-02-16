import numpy
import subprocess
import Queue
import sys
import copy


# so you want to add together all non-hole elements. 

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
	# bound should be the degree, not the element
	bound = 2*calculate_degree (missing_generator) + 2
	to_return = numpy.zeros((2*num_generators, dimension), dtype=numpy.int)
	index = 0
	index2 = 0
	for i in range(num_generators):
		of_interest = old_gen[i]
		if i != missing_index:
			to_return[index] = old_gen[i]
			index += 1
		potential = of_interest+missing_generator
		if calculate_degree (potential) >= bound:
			to_return[index:num_generators-1] = old_gen[i+1:num_generators]
			break
		to_return[num_generators-1+index2] = potential
		index2 += 1
	if calculate_degree (3*missing_generator) < bound:
		to_return[num_generators-1+index2] = 3*missing_generator
		index2 += 1
	return to_return[:num_generators-1+index2]


dimension = 2
# new strategy. you just need to check for the new generators whether or not
# they're able to be written as a sum of two elements

# iterate over all way of writing them as a sum of two elements

def next_one (current, biggest):

	length = len (current)
	#to_return = list (current)
	for i in range (length-1, -1, -1):
		if current[i] == biggest[i]:
			current[i] = 0
		else:
			current[i] += 1
			break
	if current == biggest.tolist():
		return -1
	return current


def all_sum (vector, prefix):
        # if the vector is of length 0, then print prefix
        length = len (vector)
        if length == 0:
                print prefix
                return
        for i in range (0, vector[0]+1):
                all_sum (vector[1:], prefix+[i])
        return

"""
def all_substrings (string, yes):
	length = len (string)
	if length == 0:
		return
	print string
	all_substrings (string[:-1], False)
	if yes == True:
                all_substrings (string[1:], True)
	return
"""

vector = [2,3,4,5]
start = [0,0,0,0]

# for each new potential generator, you can check if it's the sum of two elements
# you iterate through all elements less than it and check if it's a hole. if
# both of them are not holes, then you're good

def is_min_gen (generator, holes):
	# holes should be a set
	#length = len (generator)
	where_at = [0]*dimension
	where_at = next_one (where_at, generator)
	# holes should be list of lists
	# where at is list
	# TODO create hash for the holes
	#print "holes:", holes
	#print "generator:", generator
	while where_at != -1:
		#print "where at:", where_at
		if where_at in holes:
			where_at = next_one (where_at, generator)
			continue
		temp_where_at = numpy.asarray(where_at)
		complement = [generator[i]-where_at[i] for i in range (dimension)]
		#print "complement:", complement
		if complement in holes:
			where_at = next_one (where_at, generator)
			continue
		#print "complement", complement
		return False
	#print >> sys.stderr, "sldkfjsdlfj"
	return True

def get_min_gen (potential_generators, where_start, holes):
	length = len (potential_generators)
	good_ones = numpy.zeros ((length, dimension), dtype=numpy.int)
	good_ones[:where_start] = potential_generators[:where_start]
	index = where_start
	for i in range (where_start, length):
		of_interest = potential_generators[i]
		if is_min_gen (of_interest, holes) == True:
			good_ones[index] = of_interest
			index += 1
	return good_ones[:index]

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




dimension = 1
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
	old_hole_set = of_interest.hole_set
	for i in range(index_of_effective, num_generators):
		missing_generator = of_interest.generators[i]

		if old_hole_set != None:
			new_hole_set = numpy.append (old_hole_set, [missing_generator], axis=0)
		else:
			new_hole_set = numpy.asarray([missing_generator])
		list_old_holes = new_hole_set.tolist()

		potential = create_potential_generators (of_interest, i)
		#print "potential gens:", potential
		min_gens = get_min_gen (potential, num_generators-1, list_old_holes)
		#print "min gens:", min_gens
		# min_gens = get_min_gen (potential, num_generators-1, 2*calculate_degree(missing_generator)+1)
		sorted, index_of_effective = sorted_generators (min_gens, num_generators-1, missing_generator)
		total += 1
		new_gns = GNS (sorted, len(sorted), index_of_effective, missing_generator, new_hole_set, of_interest.num_holes+1, calculate_degree (missing_generator), None, of_interest.id, total)
		print_gns (new_gns)
		queue.put(new_gns)

