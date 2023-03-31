## Assignment 3
### Members:
* Sailee (2020201003)
* Sushant (2020201004)
* Mohit (2020201094)
* Raman (2020201098)

### Our Work

* There are 3 basic roles played by `Supply_Chain` , `Tag` and `Reader`.
* The communication between Supply Chain and the Reader are encrypted by a shared key XRS.
* The session role (composite role) is defined for the composition of the above roles.
* There are multiple functions used for Addition,Substraction,Rotation,Right Rotation and Hashing which are of type `hash_func`.
* Both Supply Chain and the Tag store the secret key `SKST` which is shared between these two.

### Reader Role
* This is the role which runs first. This role has 4 states (0 to 4) and 3 transitions.
* This role communicates with both supply chain as well as the tag.

### Supply Chain role
* This role has 2 states (0 and 1) and 1 transition.
* This role communicates only with the Reader.

### Tag Role
* This role has 3 states (0 to 2) and 2 transitions.
* This role communicates only with the Reader.

### Conclusion
* The code has no syntax errors and is running.