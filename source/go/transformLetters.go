package main

import (
	"fmt"
	"strings"
)

func tranformLetters(input string) (output string) {
	lower := strings.ToLower(input)
	//Iterate
	for i, c := range lower {
		//COUNT OCCURANCES
		a := 0
		for _, p := range lower {
			if p == c {
				a += 1
			} else {
				a += 0
			}
		}
		output += string((int(c)%32-1+(int(a)-1)*(int(c)-97)%26+1)%26 + int(input[i])/32*32 + 1)
	}
	return output
}

func EXPECT_EQ(exp string, given string) {
	if exp == given {
		fmt.Printf("Success!\n")
	} else {
		fmt.Printf("Failed:Expected %s but found %s\n", exp, given)
	}
}

//-----------------------------------------------------------------------------
func main() {
	EXPECT_EQ("bitl", tranformLetters("ahsk"))
	EXPECT_EQ("dgfdod", tranformLetters("cfebnb"))
	EXPECT_EQ("bExbs", tranformLetters("aDwar"))
	EXPECT_EQ("lbLabQbl", tranformLetters("faKzaPaf"))
	EXPECT_EQ("hhh", tranformLetters("ccc"))
	EXPECT_EQ("VaVaVaVu", tranformLetters("SrSrSrSt"))
	EXPECT_EQ("cfFkkkrrrr", tranformLetters("bcCdddeeee"))
	EXPECT_EQ("b", tranformLetters("a"))
	EXPECT_EQ("bB", tranformLetters("aA"))
	EXPECT_EQ("d", tranformLetters("c"))
	EXPECT_EQ("Ff", tranformLetters("Cc"))
	EXPECT_EQ("Hhh", tranformLetters("Ccc"))
	EXPECT_EQ("e", tranformLetters("d"))
	EXPECT_EQ("hh", tranformLetters("dd"))
	EXPECT_EQ("kkk", tranformLetters("ddd"))
}
