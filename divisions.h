#ifndef LIBEROS_DIVISIONS_H
#define LIBEROS_DIVISIONS_H

#include <QObject>
#include <QList>
#include <QPair>

namespace protobufs
{
	class HandshakeResponse;
}

class Divisions : public QObject
{
	Q_OBJECT

public:
	struct Division
	{
		QString name;
		int points;
	};

	Divisions(QObject *parent, const protobufs::HandshakeResponse &response);
	~Divisions();

	int size() const;
    const QList< QPair<int, QString> > &divisions() const;
	const QPair<int, QString> &division(int points) const;
	int difference(int points, int points2) const;

	static QPair<int, QString> unknownDivision;

private:
    QList< QPair<int, QString> > divisions_;
};

#endif // LIBEROS_DIVISIONS_H
