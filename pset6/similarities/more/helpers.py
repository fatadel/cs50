from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def distances(a, b):
    """Calculate edit distance from a to b"""

    # TODO
    # Create matrix len(a) + 1 by len(b) + 1
    # Populate it with base data for 0th row
    l = [[(j, Operation.INSERTED) for j in range(len(b) + 1)] for i in range(len(a) + 1)]

    # Change the base data for 0th column
    for i in range(len(l)):
        l[i][0] = (i, Operation.DELETED)

    # Change the base data for [0;0] cell
    l[0][0] = (0, None)

    # Iterate the rows starting from the 1th
    for i in range(1, len(l)):

        # Iterate the columns starting from 1th
        for j in range(1, len(l[i])):

            # Calculate cost for deletion
            dCost = (l[i - 1][j][0] + 1, Operation.DELETED)

            # Calculate cost for insertion
            iCost = (l[i][j - 1][0] + 1, Operation.INSERTED)

            # Calculate cost for substitution
            if a[i - 1] == b[j - 1]:
                sCost = (l[i - 1][j - 1][0], Operation.SUBSTITUTED)
            else:
                sCost = (l[i - 1][j - 1][0] + 1, Operation.SUBSTITUTED)

            # Populate the [i;j] cell with min cost and the appropriate operation
            # key=lambda t: t[0] means we look at the first value of a tuple (i.e. cost) when looking for min
            l[i][j] = min(dCost, iCost, sCost, key=lambda t: t[0])

    return l
