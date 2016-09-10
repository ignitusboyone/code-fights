auto caesarian(auto m, int n) {
    for (i : m)
        i = (i + 7 + n % 26) % 26 + 97;
    return m;
}