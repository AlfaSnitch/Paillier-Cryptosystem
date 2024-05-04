#include<bits/stdc++.h>
#include<boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>

using namespace std;
using namespace boost::multiprecision;

cpp_int getRandomR(const cpp_int& n) {
    // Use a random number generator to generate a random number within the range [1, n)
    boost::random::mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    boost::random::uniform_int_distribution<cpp_int> dist(1, n - 1);
    return dist(gen);
}

cpp_int modInverse(cpp_int A, cpp_int M)
{
    cpp_int m0 = M;
    cpp_int y = 0, x = 1;

    if (M == 1)
        return 0;

    while (A > 1) {
        cpp_int q = A / M;
        cpp_int t = M;

        M = A % M, A = t;
        t = y;

        y = x - q * y;
        x = t;
    }

    if (x < 0)
        x += m0;

    return x;
}
// Function for extended Euclidean Algorithm
cpp_int gcdExtended(cpp_int a, cpp_int b, cpp_int* x, cpp_int* y)
{
    // Base Case
    if (a == 0) {
        *x = 0, *y = 1;
        return b;
    }
    // To store results of recursive call
    cpp_int x1, y1;
    cpp_int gcd = gcdExtended(b % a, a, &x1, &y1);
    // Update x and y using results of recursive
    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}

// calculate (b^p % mod)
cpp_int power(cpp_int b, cpp_int p, cpp_int mod)
{
    b = b % mod;
    if(p == 0)
        return 1;
    cpp_int j = power(b, p / 2, mod);
    j = (j * j) % mod;
    if(p % 2 == 1)
        j = (j * b) % mod;
    return j;
}
// the L(x,n) = (x-1)/n
cpp_int L(cpp_int x, cpp_int n){
    return (x-1)/n;
}

void keygeneration(cpp_int p,cpp_int q,cpp_int& n,cpp_int& g,cpp_int& lamda, cpp_int& miu){
    cpp_int dumx,dumy,phi;

    n = p*q;

    phi = (p-1)*(q-1);

    lamda = phi / gcdExtended(p-1,q-1,&dumx,&dumy); // lcm(a,b) = (a*b)/gcd(a,b)

    g = n+1;

    cpp_int t1 = power(g,lamda,n*n);
    t1 = L(t1,n);
    miu = modInverse(t1,n);

    cout<<"Key generation successful \n"<<endl;
    cout<<"Public Key(n,g): "<<n<<endl<<g<<endl<<endl;
    cout<<"Private key(lamda,miu): "<<endl<<lamda<<endl<<miu<<endl<<endl;
}
// for encryption
cpp_int encryption(cpp_int m,cpp_int g, cpp_int n){
    // any random variable r
    cpp_int r = getRandomR(n);
    cpp_int dumx,dumy;
    while(gcdExtended(r,n,&dumx,&dumy)!=1){
        r = getRandomR(n);
    }
    // ci = (g^m)*(r^n) (mod n^2) = ((g^m mod n^2) * (r^n mod n^2)) mod n^2
    cpp_int t1 = power(g,m,n*n);
    cpp_int t2 = power(r,n,n*n);
    cpp_int cipher_text = t1*t2 % (n*n);
    return cipher_text;
}

cpp_int decryption(cpp_int ci, cpp_int lamda, cpp_int miu, cpp_int n){
    cpp_int t1 = power(ci,lamda,n*n);
    t1 = L(t1,n);

    cpp_int decm = t1*miu % n;
    cout<<"Decrypted Message: "<<decm<<endl;
    return decm;
}

int main()
{
    cpp_int n,g;
    cpp_int dumx,dumy,lamda,miu;
    // prime number p and q
    cpp_int p("656692050181897513638241554199181923922955921760928836766304161790553989228223793461834703506872747071705167995972707253940099469869516422893633357693");
    cpp_int q("204616454475328391399619135615615385636808455963116802820729927402260635621645177248364272093977747839601125961863785073671961509749189348777945177811");

    //key generation
    keygeneration(p,q,n,g,lamda,miu);

    //encryption
    cpp_int m = 123;
    cpp_int ci = encryption(m, g, n);

    //decryption
    decryption(ci,lamda,miu,n);

    return 0;
}

