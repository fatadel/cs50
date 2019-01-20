from nltk.tokenize import sent_tokenize


def substr(s, n):
    """Return set of substrings of length n in string s"""
    # Create empty set
    l = set()
    # Iterate over the string
    for i in range(len(s)):
        # If substring needs more characters than we have - finish
        if i + n > len(s):
            break
        # Else add a new substring from (inclusive) i to i+n (exclusive) to the set
        l.add(s[i:i+n])
    return l


def lines(a, b):
    """Return lines in both a and b"""
    # TODO
    l1 = set(a.splitlines())
    l2 = set(b.splitlines())
    return l1 & l2


def sentences(a, b):
    """Return sentences in both a and b"""
    # TODO
    l1 = set(sent_tokenize(a))
    l2 = set(sent_tokenize(b))
    return l1 & l2


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    # TODO
    return substr(a, n) & substr(b, n)