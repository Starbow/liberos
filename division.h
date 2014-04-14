#ifndef LIBEROS_DIVISION_H
#define LIBEROS_DIVISION_H

#include <QObject>
namespace protobufs
{
	class Division;
}
class Division : public QObject
{
	Q_OBJECT

public:
	Division(QObject *parent, const protobufs::Division &division);
	~Division();
	Division(QObject *parent);

	int id() const;
	const QString &name() const;
	int promotionThreshold() const;
	int demotionThreshold() const;
	const QString &iconUrl() const;
	const QString &smallIconUrl() const;

	static Division *placementDivision();
private:
	

	int id_;
	QString name_;
	int promotion_threshold_;
	int demotion_threshold_;
	QString icon_url_;
	QString small_icon_url_;
};

#endif // LIBEROS_DIVISION_H
