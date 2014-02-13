#include "divisions.h"
#include "eros.pb.h"

struct QPairFirstComparer
{
    template<typename T1, typename T2>
    bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const
    {
        return a.first < b.first;
    }
};

QPair<int, QString> Divisions::unknownDivision = qMakePair<int, QString>(0, "Unknown Divisions");

Divisions::Divisions(QObject *parent, const protobufs::HandshakeResponse &response)
	: QObject(parent)
{
	this->divisions_ = QList<QPair<int, QString>>();

	for (int i = 0; i < response.division_size(); i++)
	{
		const protobufs::Division &division = response.division(i);
		this->divisions_ << qMakePair((int)division.points(), QString::fromStdString(division.name()));
	}


	qSort(this->divisions_.begin(), this->divisions_.end(), QPairFirstComparer());
}

Divisions::~Divisions()
{

}

int Divisions::size() const
{
	return this->divisions_.size();
}

const QList<QPair<int, QString>> &Divisions::divisions() const
{
	return this->divisions_;
}

const QPair<int, QString> &Divisions::division(int points) const
{
	QList<QPair<int, QString>>::const_iterator i;
	for (i = this->divisions_.end(); i != this->divisions_.begin(); i--)
	{
		if (points >= (*i).first)
		{
			return (*i);
		}
	}

	return unknownDivision;
}

int Divisions::difference(int points, int points2) const
{
	if (points < 0 || points2 < 0)
		return 0;

	const QPair<int, QString> &division1 = division(points);
	const QPair<int, QString> &division2 = division(points2);

	if (!divisions_.contains(division1) || !divisions_.contains(division2))
		return 0;
	

	int index1 = divisions_.indexOf(division1);
	int index2 = divisions_.indexOf(division2);

	return index2 - index1;
}