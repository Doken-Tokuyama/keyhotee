#include "MailViewer.hpp"

#include "ui_MailViewer.h"

#include "KeyhoteeMainWindow.hpp"
#include "MailboxModel.hpp"
#include "utils.hpp"

#include <QImageReader>
#include <QToolBar>

MailViewer::MailViewer(QWidget* parent)
: /*QWidget(parent),*/ ui( new Ui::MailViewer() )
  {
  ui->setupUi(this);
  message_tools = new QToolBar(ui->toolbar_container);
  QGridLayout* grid_layout = new QGridLayout(ui->toolbar_container);
  grid_layout->setContentsMargins(0, 0, 0, 0);
  grid_layout->setSpacing(0);
  ui->toolbar_container->setLayout(grid_layout);
  grid_layout->addWidget(message_tools, 0, 0);
  ui->message_content->setOpenExternalLinks (true);
  }

MailViewer::~MailViewer()
  {
  delete ui;
  }

void MailViewer::displayMailMessage(const QModelIndex& index, MailboxModel* mailbox)
  {
  assert(index.isValid());
  assert(mailbox != nullptr);

  //TODO: later, possibly set a timer and only mark as read if still displaying
  //      this message when timer expires?
  mailbox->markMessageAsRead(index);
  MessageHeader msg;
  mailbox->getFullMessage(index, msg);
  QString       formatted_date = msg.date_sent.toString(Qt::DefaultLocaleShortDate);
  ui->date_label->setText(formatted_date);
  ui->from_label->setText(msg.from);
  if (msg.to_list.size())
    {
    ui->to_prefix->show();
    ui->to_label->show();
    ui->to_label->setText(Utils::makeContactListString(msg.to_list, ';', Utils::FULL_CONTACT_DETAILS));
    }
  else
    {
    ui->to_prefix->hide();
    ui->to_label->hide();
    }
  if (msg.cc_list.size())
    {
    ui->cc_prefix->show();
    ui->cc_label->show();
    ui->cc_label->setText(Utils::makeContactListString(msg.cc_list, ';', Utils::FULL_CONTACT_DETAILS));
    }
  else
    {
    ui->cc_prefix->hide();
    ui->cc_label->hide();
    }
  //TODO: add to and cc lists
  ui->subject_label->setText(msg.subject);
  ui->message_content->setHtml(msg.body);
  }

