#include "Common.h"
#include "test_runner.h"

#include <sstream>

using namespace std;

class Digits : public Expression {
public:
	Digits() = default;
	Digits(int val_) : val(val_) {}

	virtual int Evaluate() const override {
		return val;
	}

	virtual std::string ToString() const override {
		return to_string(val);
	}

private:
	  int val = 0;
};

class Operation : public Expression {
public:
	Operation(ExpressionPtr left, ExpressionPtr right) : left(move(left)), right(move(right)) {}

	virtual int Evaluate() const final {
		return EvaluateOnValues(left->Evaluate(), right->Evaluate());
	}

	virtual std::string ToString() const override {
		ostringstream out;
		out << '(' <<  left->ToString() << ')' << GetSymbol() << '(' << right->ToString() << ')';
		return out.str();
	}

private:
	virtual char GetSymbol() const = 0;
	virtual int EvaluateOnValues(int l, int r) const = 0;
	ExpressionPtr left, right;

};
class Multi : public Operation {
public:
	Multi(ExpressionPtr left, ExpressionPtr right) : Operation(move(left), move(right)) {}

private:
	virtual int EvaluateOnValues(int l, int r) const override {
		return l * r;
	}
	char GetSymbol() const override {
		return '*';
	}


};
class Plus : public Operation {
public:
	Plus(ExpressionPtr left, ExpressionPtr right) : Operation(move(left), move(right)) {}

private:
	virtual int EvaluateOnValues(int l, int r) const override {
		return l + r;
	}
	char GetSymbol() const override {
		return '+';
	}

};


ExpressionPtr Value(int value) {
	return make_unique<Digits>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
	return make_unique<Plus>(move(left), move(right));

}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
	return make_unique<Multi>(move(left), move(right));
}


string Print(const Expression* e) {
  if (!e) {
    return "Null expression provided";
  }
  stringstream output;
  output << e->ToString() << " = " << e->Evaluate();
  return output.str();
}

void Test() {
  ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
  ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

  ExpressionPtr e2 = Sum(move(e1), Value(5));
  ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

  ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, Test);
  return 0;
}
