#include <iostream>
#include <iomanip>
using namespace std;

size_t tab = 40;
const size_t tab2 = 7;

struct decimal {
	uint8_t nums[10];
	uint32_t num32;
	uint8_t* ptr;
	decimal(uint32_t n) { set(n); }
	void operator= (uint32_t n) { set(n); }
	void set(uint32_t n) {
		num32 = n;
		memset(nums, 0, sizeof(nums));
		ptr = nums;
		while (n) {
			*ptr++ = n % 10;
			n /= 10;
		}
	}
};

static ostream& printTab(size_t tab) {
	static const char c0 = 0;
	return cout << setw(tab) << &c0;
}

static uint32_t printDiv(decimal* a, const decimal* b, size_t* tab_ptr) {
	uint8_t* a_ptr = a->ptr;
	uint32_t a32 = 0;
	uint32_t b32 = b->num32;
	uint32_t q = a->num32 / b32;
	size_t alen = a->ptr - a->nums;
	size_t tab = *tab_ptr - (alen + 1 + b->ptr-b->nums);
	size_t tab2 = tab;
	bool begin = true;
	//size_t w = 0;

	char* const line = (char*)_alloca(alen + 2);
	memset(line, '-', alen);
	line[alen] = '\n';
	line[alen + 1] = 0;

	do {
		do {
			a_ptr--;
			a32 = a32*10 + *a_ptr;
			tab2++;
		} while (a32 < b32 && a_ptr != a->nums);

		//*c.ptr++ = a32 / b32;
		uint32_t r32 = a32 % b32;

		if (begin) {
			begin = false;
			printTab(tab) << a->num32 << '|' << b32 << '\n';
			printTab(tab) << left << setw(alen) << a32 - r32 << '|' << q << '\n' << right;
		}
		else {
			cout << setw(tab2) << a32 << '\n';
			cout << setw(tab2) << a32 - r32 << '\n' << setw(tab + alen);
		}
		//printTab(tab) << line;

		a32 = r32;
	} while (a_ptr != a->nums);

	*tab_ptr = tab2;

	printTab(tab) << line;

	*a = a32;
	//cout << setw(w) << a32 << '\n';
	return q;
}


int main()
{
	uint32_t a, b;
	cin >> a >> b;
	system("cls");
	if (a < b) swap(a, b);
	decimal d[2] = { a,b };
	decimal* da = d;
	decimal* db = d + 1;

	uint32_t q[16];
	uint32_t* q_ptr = q;
	while (true) {
		uint32_t da_db = printDiv(da, db, &tab);
		if (!da->num32) break;
		*q_ptr++ = da_db;
		swap(da, db);
	}
	cout << setw(tab+2) << "0\n\n";



	uint32_t qlen = q_ptr - q;
	cout << setw(tab2) << 'j';
	for (int i = 0; i <= qlen; i++)
		cout << setw(tab2) << i;
	cout << '\n';

	cout << setw(tab2) << "Qj" << setw(tab2) << '-';
	uint32_t* qend = q_ptr;
	q_ptr = q;
	do cout << setw(tab2) << *q_ptr;
	while (++q_ptr != qend);
	cout << '\n';

	cout << setw(tab2) << "Kj" << setw(tab2) << '1';
	uint32_t k[] = { 0,1 };
	for (int i = 0; i < qlen; i++) {
		uint32_t k2 = q[i] * k[1] + k[0];
		cout << setw(tab2) << k2;
		k[0] = k[1];
		k[1] = k2;
	}
	cout << '\n';
	int u[] = { k[1], (qlen&1 ? -k[1] : k[1]) };


	cout << setw(tab2) << "Kj-1" << setw(tab2) << '-' << setw(tab2) << '1';
	k[0] = 0;
	k[1] = 1;
	for (int i = 1; i < qlen; i++) {
		uint32_t k2 = q[i] * k[1] + k[0];
		cout << setw(tab2) << k2;
		k[0] = k[1];
		k[1] = k2;
	}
	cout << "\n\n";
	int v[] = { k[1], (qlen&1 ? k[1] : -k[1]) };


	cout << "v = (-1) ^ (" << qlen << "-1) * " << v[0] << " = " << v[1] << '\n';
	cout << "u = (-1) ^ " << qlen << " * " << u[0] << " = " << u[1] << '\n';
	cout << "GCD = "
		<< v[1] << " * " << a << (u[1]&(1<<31) ? " - " : " + ") << u[0] << " * " << b << " = "
		<< v[1] * (int)a + u[1] * (int)b << "\n\n";


	system("pause");
	return 0;
}
