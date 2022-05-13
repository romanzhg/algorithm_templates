// Rational number.

typedef long long ll;

// TODO: clarify on why this value is chosen.
const ll LL_INF = 0x3f3f3f3f;

// Greatest common divisor.
ll gcd(ll a, ll b) {
	return b == 0 ? a : gcd(b, a % b);
}

// A rational number represented by a / b.
struct Rational {
	ll a;
	ll b;

	Rational(ll ia = 0, ll ib = 1) {
		ll divisor = gcd(ia, ib);
		ia = ia / divisor;
		ib = ib / divisor;
		if (ib < 0) {
			ia = -ia;
			ib = -ib;
		}
		this->a = ia;
		this->b = ib;
	};
};

Rational operator + (const Rational& l, const Rational& r) {
	ll divisor = gcd(l.b, r.b);
  return Rational(l.a * (r.b / divisor) + r.a * (l.b / divisor), l.b * (r.b / divisor));
}

Rational operator - (const Rational& l, const Rational& r) {
	ll divisor = gcd(l.b, r.b);
  return Rational(l.a * (r.b / divisor) - r.a * (l.b / divisor), l.b * (r.b / divisor));
}

Rational operator * (const Rational& l, const Rational& r) {
  return Rational(l.a * r.a, l.b * r.b);
}

Rational operator * (const Rational& l, const ll& r) {
  return Rational(l.a * r, l.b);
}

Rational operator / (const Rational& l, const Rational& r) {
  return Rational(l.a * r.b, r.a * l.b);
}

// This operator only works when both l and r are larger than 0.
// TODO: have a more general version.
bool operator < (const Rational& l, const Rational& r) {
  return l.a * r.b < r.a * l.b;
}