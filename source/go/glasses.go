//Author ignitus
//Date   2016-09-19
//File   glasses.go
//Given Three Jugs Find the count of unique volumes obtainable
//by any combination of the following two opertations
//Empty(x)  -> Sets Jugs Volume to 0
//Transfer(x,y) -> Moves a units from X to Y such that Y is full but not overflown.
package main

import (
	"fmt"
	"sort"
)

//-----------------------------------------------------------------------------
type Glass struct {
	volume   int
	capacity int
}
type Glasses []Glass

//-----------------------------------------------------------------------------
func CreateGlass(cap int) Glass {
	return Glass{cap, cap}
}

//-----------------------------------------------------------------------------
func empty_glass(state Glasses, index int) Glasses {
	if len(state) < 3 {
		return Glasses{}
	}
	if state[index].volume != 0 {
		var new_state Glasses
		new_state = append(new_state, state[0])
		new_state = append(new_state, state[1])
		new_state = append(new_state, state[2])
		new_state[index].volume = 0
		if (new_state[0].volume != 0) || (new_state[1].volume != 0) || (new_state[2].volume != 0) {
			// fmt.Printf("empty_glass=%v\n", new_state)
			return new_state
		} else {
			return Glasses{}
		}
	}
	return Glasses{}
}

//-----------------------------------------------------------------------------
func glass_transfer(state Glasses, from int, to int) Glasses {
	if len(state) < 3 {
		return Glasses{}
	}
	if state[from].volume != 0 && state[to].capacity != state[to].volume {
		var new_state Glasses
		new_state = append(new_state, state[0])
		new_state = append(new_state, state[1])
		new_state = append(new_state, state[2])
		vaccent := state[to].capacity - state[to].volume
		avaliable := state[from].volume
		if vaccent >= avaliable {
			new_state[from].volume = 0
			new_state[to].volume += avaliable
		} else {
			avaliable -= vaccent
			new_state[from].volume -= vaccent
			new_state[to].volume += vaccent
		}
		return new_state
	}
	return Glasses{}
}

//-----------------------------------------------------------------------------
func accum_ints(data []int) int {
	ans := int(0)
	for _, e := range data {
		ans += e
	}
	return ans
}

//-----------------------------------------------------------------------------
func accum_glasses(data []Glass) int {
	ans := int(0)
	for _, e := range data {
		ans += e.volume
	}
	return ans
}

//-----------------------------------------------------------------------------
func glass_key(state Glasses) int {
	if len(state) == 3 {
		return state[0].volume<<32 | state[1].volume<<16 | state[2].volume
	} else {
		return 0
	}
}

func threeGlasses(sizes []int) int {
	work_queue := make([]Glasses, 0)
	volumes := make(map[int]int)
	states := make(map[int]int)
	ans := int(1)
	sort.Ints(sizes)
	if sizes[0] == 1 {
		ans = accum_ints(sizes)
	} else {
		volumes[accum_ints(sizes)]++

		work_queue = append(work_queue, Glasses{Glass{0, sizes[0]}, Glass{sizes[1], sizes[1]}, Glass{sizes[2], sizes[2]}})
		work_queue = append(work_queue, Glasses{Glass{sizes[0], sizes[0]}, Glass{0, sizes[1]}, Glass{sizes[2], sizes[2]}})
		work_queue = append(work_queue, Glasses{Glass{sizes[0], sizes[0]}, Glass{sizes[1], sizes[1]}, Glass{0, sizes[2]}})
		states[glass_key(Glasses{Glass{sizes[0], sizes[0]}, Glass{sizes[1], sizes[1]}, Glass{sizes[2], sizes[2]}})]++

		for len(work_queue) != 0 {
			current := work_queue[0]
			if len(current) == 0 {
				work_queue = work_queue[1:]
				continue
			}
			// fmt.Printf("key = %d state=%v  = %d\n", glass_key(current), current, accum_glasses(current))
			volumes[accum_glasses(current)]++
			new_state := glass_transfer(current, 0, 1)
			if states[glass_key(new_state)] == 0 {
				states[glass_key(new_state)]++
				work_queue = append(work_queue, new_state)
				// fmt.Printf("new_state=%v\n", new_state)
				work_queue = append(work_queue, empty_glass(new_state, 0))
				work_queue = append(work_queue, empty_glass(new_state, 1))
				work_queue = append(work_queue, empty_glass(new_state, 2))
			}
			new_state = glass_transfer(current, 0, 2)
			if states[glass_key(new_state)] == 0 {
				states[glass_key(new_state)]++
				work_queue = append(work_queue, new_state)
				// fmt.Printf("new_state=%v\n", new_state)
				work_queue = append(work_queue, empty_glass(new_state, 0))
				work_queue = append(work_queue, empty_glass(new_state, 1))
				work_queue = append(work_queue, empty_glass(new_state, 2))
			}
			new_state = glass_transfer(current, 1, 0)
			if states[glass_key(new_state)] == 0 {
				states[glass_key(new_state)]++
				work_queue = append(work_queue, new_state)
				// fmt.Printf("new_state=%v\n", new_state)
				work_queue = append(work_queue, empty_glass(new_state, 0))
				work_queue = append(work_queue, empty_glass(new_state, 1))
				work_queue = append(work_queue, empty_glass(new_state, 2))
			}
			new_state = glass_transfer(current, 1, 2)
			if states[glass_key(new_state)] == 0 {
				states[glass_key(new_state)]++
				work_queue = append(work_queue, new_state)
				// fmt.Printf("new_state=%v\n", new_state)
				work_queue = append(work_queue, empty_glass(new_state, 0))
				work_queue = append(work_queue, empty_glass(new_state, 1))
				work_queue = append(work_queue, empty_glass(new_state, 2))
			}
			new_state = glass_transfer(current, 2, 0)
			if states[glass_key(new_state)] == 0 {
				states[glass_key(new_state)]++
				work_queue = append(work_queue, new_state)
				// fmt.Printf("new_state=%v\n", new_state)
				work_queue = append(work_queue, empty_glass(new_state, 0))
				work_queue = append(work_queue, empty_glass(new_state, 1))
				work_queue = append(work_queue, empty_glass(new_state, 2))
			}
			new_state = glass_transfer(current, 2, 1)
			if states[glass_key(new_state)] == 0 {
				states[glass_key(new_state)]++
				work_queue = append(work_queue, new_state)
				// fmt.Printf("new_state=%v\n", new_state)
				work_queue = append(work_queue, empty_glass(new_state, 0))
				work_queue = append(work_queue, empty_glass(new_state, 1))
				work_queue = append(work_queue, empty_glass(new_state, 2))
			}
			work_queue = work_queue[1:]
			ans = len(volumes)
			// for (auto& ans: volumes){
			//     std::cout << "( "<< ans.first << ", " << ans.second << " )" << "\n";
			// }
		}
	}
	return ans
}

//-----------------------------------------------------------------------------
func EXPECT_EQ(exp int, given int) {
	if exp == given {
		fmt.Printf("Success!\n")
	} else {
		fmt.Printf("Failed:Expected %3d but found %3d\n", exp, given)
	}
}

//-----------------------------------------------------------------------------
func main() {
	EXPECT_EQ(21, threeGlasses([]int{16, 5, 3}))
	EXPECT_EQ(3, threeGlasses([]int{1, 1, 1}))
	EXPECT_EQ(151, threeGlasses([]int{1, 50, 100}))
	EXPECT_EQ(13, threeGlasses([]int{8, 5, 3}))
	EXPECT_EQ(6, threeGlasses([]int{9, 6, 3}))
	EXPECT_EQ(3, threeGlasses([]int{3, 3, 3}))
	EXPECT_EQ(3, threeGlasses([]int{7, 7, 7}))
	EXPECT_EQ(7, threeGlasses([]int{7, 3, 3}))
	EXPECT_EQ(11, threeGlasses([]int{19, 3, 3}))
	EXPECT_EQ(9, threeGlasses([]int{21, 3, 3}))
	EXPECT_EQ(15, threeGlasses([]int{21, 21, 3}))
	EXPECT_EQ(7, threeGlasses([]int{8, 4, 2}))
	EXPECT_EQ(13, threeGlasses([]int{7, 7, 3}))
	EXPECT_EQ(7, threeGlasses([]int{99, 66, 99}))
	EXPECT_EQ(22, threeGlasses([]int{47, 6, 3}))
	EXPECT_EQ(104, threeGlasses([]int{41, 61, 79}))
	EXPECT_EQ(200, threeGlasses([]int{98, 99, 100}))
}
