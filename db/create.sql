spool create_table.log
prompt
prompt Creating table TG_ACCMAP
prompt =========================
prompt

drop table TG_ACCMAP;
create table TG_ACCMAP
(
  refid       CHAR(32)     not null,
  platformid  CHAR(8)      not null,
  accountcode NUMBER(19)   not null,
  acccode     CHAR(19)     not null,
  cardid      NUMBER(19)   not null,
  trackcardno VARCHAR2(18) not null,
  custid      NUMBER(19)   not null,
  status      CHAR(1)      not null,
  createdate  CHAR(8)      not null
)
;
comment on table TG_ACCMAP
  is '�й�ҵ���˺�ӳ���';
comment on column TG_ACCMAP.refid is '�ⲿ��ʶ��';
comment on column TG_ACCMAP.platformid is 'ƽ̨��';
comment on column TG_ACCMAP.accountcode is '�˻�����';
comment on column TG_ACCMAP.acccode is '���˻�����';
comment on column TG_ACCMAP.cardid is '��Ƭid';
comment on column TG_ACCMAP.trackcardno is '���濨��';
comment on column TG_ACCMAP.custid is '�ͻ���';
comment on column TG_ACCMAP.status is '״̬';
comment on column TG_ACCMAP.createdate is '��������';

alter table TG_ACCMAP
  add constraint PK_TG_ACCMAP primary key (refid, platformid);
  
drop table TG_ORGANINFO;
create table TG_ORGANINFO
(
  instcode       CHAR(8)      not null,
  platformid     CHAR(8)      not null,
  instname       VARCHAR2(80),
  linkname       VARCHAR2(20),
  address        VARCHAR2(120),
  tel            VARCHAR2(40),
  zipcode        VARCHAR2(6),
  email          VARCHAR2(40),
  remark         VARCHAR2(40),
  status         CHAR(2)     not null,
  createdate     CHAR(8)     not null
)
;
comment on table TG_ORGANINFO
  is '�й�ҵ�������Ϣ��';
comment on column TG_ORGANINFO.instcode   is '������';
comment on column TG_ORGANINFO.platformid is 'ƽ̨��';
comment on column TG_ORGANINFO.instname   is '������';
comment on column TG_ORGANINFO.linkname   is '��ϵ��';
comment on column TG_ORGANINFO.address    is '��ַ';
comment on column TG_ORGANINFO.tel        is '�绰';
comment on column TG_ORGANINFO.zipcode    is '�ʱ�';
comment on column TG_ORGANINFO.email      is 'email';
comment on column TG_ORGANINFO.remark     is '��ע';
comment on column TG_ORGANINFO.status     is '����״̬';
comment on column TG_ORGANINFO.createdate is '��������';

alter table TG_ORGANINFO
  add constraint PK_TG_ORGANINFO primary key (instcode, platformid);
  
drop table TG_INSTACCINFO;
create table TG_INSTACCINFO
(
  instcode       CHAR(8)      not null,
  platformid     CHAR(8)      not null,
  rechargetotamt NUMBER(18,0) not null,
  rechargetotnum NUMBER(8)    not null,
  transouttotamt NUMBER(18,0) not null,
  transouttotnum NUMBER(8)    not null,
  transintotamt  NUMBER(18,0) not null,
  transintotnum  NUMBER(8)    not null,
  feetotamt      NUMBER(18,0) not null,
  feetotnum      NUMBER(8)    not null,
  amout          NUMBER(18,0) not null,
  balancemd5     CHAR(32)     not null
)
;
comment on table TG_INSTACCINFO
  is '�й�ҵ������˻���(����)';
comment on column TG_INSTACCINFO.instcode       is '������';
comment on column TG_INSTACCINFO.platformid     is 'ƽ̨��';
comment on column TG_INSTACCINFO.rechargetotamt is '�ۼƳ�ֵ�ܽ��';
comment on column TG_INSTACCINFO.rechargetotnum is '�ۼƳ�ֵ�ܱ���';
comment on column TG_INSTACCINFO.transouttotamt is '�ۼ�ת���ܽ��';
comment on column TG_INSTACCINFO.transouttotnum is '�ۼ�ת���ܱ���';
comment on column TG_INSTACCINFO.transintotamt  is '�ۼ�ת���ܽ��';
comment on column TG_INSTACCINFO.transintotnum  is '�ۼ�ת���ܱ���';
comment on column TG_INSTACCINFO.feetotamt      is '�ۼ���ѯ���ܽ��';
comment on column TG_INSTACCINFO.feetotnum      is '�ۼ���ѯ���ܱ���';
comment on column TG_INSTACCINFO.amout          is '�������';
comment on column TG_INSTACCINFO.balancemd5     is '����md5����';

alter table TG_INSTACCINFO
  add constraint PK_TG_INSTACCINFO primary key (instcode, platformid);
  
drop table TG_PLATACCINFO;
create table TG_PLATACCINFO
(
  platformid      CHAR(8)      not null,
  rechargetotamt  NUMBER(18,0) not null,
  rechargetotnum  NUMBER(8)    not null,
  cashtotamt      NUMBER(18,0) not null,
  cashtotnum      NUMBER(8)    not null,
  incometotamt    NUMBER(18,0) not null,
  incometotnum    NUMBER(8)    not null,
  extractedtotamt NUMBER(18,0) not null,
  extractedtotnum NUMBER(8)    not null,
  feeamount       NUMBER(18,0) not null,
  feebalancemd5   CHAR(32)     not null,
  amout           NUMBER(18,0) not null,
  balancemd5      CHAR(32)     not null
)
;
comment on table TG_PLATACCINFO
  is '�й�ҵ��ƽ̨�˻���';
comment on column TG_PLATACCINFO.platformid      is 'ƽ̨��';
comment on column TG_PLATACCINFO.rechargetotamt  is '�ۼƳ�ֵ�ܽ��';
comment on column TG_PLATACCINFO.rechargetotnum  is '�ۼƳ�ֵ�ܱ���';
comment on column TG_PLATACCINFO.cashtotamt      is '�ۼ������ܽ��';
comment on column TG_PLATACCINFO.cashtotnum      is '�ۼ������ܱ���';
comment on column TG_PLATACCINFO.incometotamt    is '�ۼ������ܽ��';
comment on column TG_PLATACCINFO.incometotnum    is '�ۼ������ܱ���';
comment on column TG_PLATACCINFO.extractedtotamt is '����ȡ�����ܽ��';
comment on column TG_PLATACCINFO.extractedtotnum is '����ȡ�����ܱ���';
comment on column TG_PLATACCINFO.feeamount       is '�������';
comment on column TG_PLATACCINFO.feebalancemd5   is '��������md5����';
comment on column TG_PLATACCINFO.amout           is '���';
comment on column TG_PLATACCINFO.balancemd5      is '����md5����';

alter table TG_PLATACCINFO
  add constraint PK_TG_PLATACCINFO primary key (platformid);
  
drop table TG_PLATFORMINFO;
create table TG_PLATFORMINFO
(
  platformid     CHAR(8)      not null,
  platformtype   CHAR(2)      not null,
  platformname   VARCHAR2(80) not null,
  linkname       VARCHAR2(20),
  address        VARCHAR2(120),
  tel            VARCHAR2(40),
  zipcode        VARCHAR2(6),
  email          VARCHAR2(40),
  remark         VARCHAR2(40),
  status         CHAR(2)      not null,
  createdate     CHAR(8)      not null
)
;
comment on table TG_PLATFORMINFO
  is '�й�ҵ�������Ϣ��';
comment on column TG_PLATFORMINFO.platformid   is 'ƽ̨��';
comment on column TG_PLATFORMINFO.platformtype is 'ƽ̨����';
comment on column TG_PLATFORMINFO.platformname is 'ƽ̨����';
comment on column TG_PLATFORMINFO.linkname     is '��ϵ��';
comment on column TG_PLATFORMINFO.address      is '��ַ';
comment on column TG_PLATFORMINFO.tel          is '�绰';
comment on column TG_PLATFORMINFO.zipcode      is '�ʱ�';
comment on column TG_PLATFORMINFO.email        is 'email';
comment on column TG_PLATFORMINFO.remark       is '��ע';
comment on column TG_PLATFORMINFO.status       is '����״̬';
comment on column TG_PLATFORMINFO.createdate   is '��������';

alter table TG_PLATFORMINFO
  add constraint PK_TG_PLATFORMINFO primary key (platformid);
  
drop table TG_ACCOUNTFLOWP;
create table TG_ACCOUNTFLOWP
(
  referenceno  CHAR(18)     not null,
  paltformid   CHAR(8)      not null,
  transcode    CHAR(4)      not null,
  reftxnno     CHAR(5)      not null,
  channelcode  CHAR(8)      not null,
  termcode     CHAR(8)      not null,
  accountid    NUMBER(19)   not null,
  accountname  VARCHAR2(20),
  linkaccid    NUMBER(19),
  linkaccname  VARCHAR2(20),
  acccode      CHAR(19)     not null,
  linkacccode  CHAR(19),
  commdate     CHAR(8),
  commtime     CHAR(6),
  transdate    CHAR(8),
  transtime    CHAR(6),
  amount       NUMBER(18,0) not null,
  fee          NUMBER(18,0) not null,
  status       CHAR(2)      not null,
  paycode      CHAR(2)      not null,
  txnid        CHAR(16)     not null
)
;
comment on table TG_ACCOUNTFLOWP
  is '�й�ҵ��������ˮ��(����)';
comment on column TG_ACCOUNTFLOWP.referenceno is '��ˮ��';
comment on column TG_ACCOUNTFLOWP.paltformid  is 'ƽ̨��';
comment on column TG_ACCOUNTFLOWP.transcode   is '���״���';
comment on column TG_ACCOUNTFLOWP.reftxnno    is '�ⲿ���״���';
comment on column TG_ACCOUNTFLOWP.channelcode is '��������';
comment on column TG_ACCOUNTFLOWP.termcode    is '�ն˴���';
comment on column TG_ACCOUNTFLOWP.accountid   is '�˻�����';
comment on column TG_ACCOUNTFLOWP.accountname is '�˻���';
comment on column TG_ACCOUNTFLOWP.linkaccid   is '�����˻�����';
comment on column TG_ACCOUNTFLOWP.linkaccname is '�����˻���';
comment on column TG_ACCOUNTFLOWP.acccode     is '���˻�����';
comment on column TG_ACCOUNTFLOWP.linkacccode is '�������˻�����';
comment on column TG_ACCOUNTFLOWP.commdate    is 'ͨѶ����';
comment on column TG_ACCOUNTFLOWP.commtime    is 'ͨѶʱ��';
comment on column TG_ACCOUNTFLOWP.transdate   is '��������';
comment on column TG_ACCOUNTFLOWP.transtime   is '����ʱ��';
comment on column TG_ACCOUNTFLOWP.amount      is '���׽��';
comment on column TG_ACCOUNTFLOWP.fee         is '��ѯ��';
comment on column TG_ACCOUNTFLOWP.status      is '����״̬';
comment on column TG_ACCOUNTFLOWP.paycode     is '���㷽ʽ����';
comment on column TG_ACCOUNTFLOWP.txnid       is '�ⲿϵͳ��ˮ��';

alter table TG_ACCOUNTFLOWP
  add constraint PK_TG_ACCOUNTFLOWP primary key (referenceno);
  
drop table TG_ACCOUNTFLOWO;
create table TG_ACCOUNTFLOWO
(
  referenceno  CHAR(18)     not null,
  paltformid   CHAR(8)      not null,
  instcode     CHAR(8)      not null,
  transcode    CHAR(4)      not null,
  reftxnno     CHAR(5)      not null,
  channelcode  CHAR(8)      not null,
  termcode     CHAR(8)      not null,
  accountid    NUMBER(19)   not null,
  accountname  VARCHAR2(20),
  linkaccid    NUMBER(19),
  linkaccname  VARCHAR2(20),
  acccode      CHAR(19)     not null,
  linkacccode  CHAR(19),
  commdate     CHAR(8),
  commtime     CHAR(6),
  transdate    CHAR(8),
  transtime    CHAR(6),
  amount       NUMBER(18,0) not null,
  fee          NUMBER(18,0) not null,
  status       CHAR(2)      not null,
  paycode      CHAR(2)      not null,
  txnid        CHAR(16)     not null
)
;
comment on table TG_ACCOUNTFLOWO
  is '�й�ҵ��������ˮ��(����)';
comment on column TG_ACCOUNTFLOWO.referenceno is '��ˮ��';
comment on column TG_ACCOUNTFLOWO.paltformid  is 'ƽ̨��';
comment on column TG_ACCOUNTFLOWO.instcode    is '������';
comment on column TG_ACCOUNTFLOWO.transcode   is '���״���';
comment on column TG_ACCOUNTFLOWO.reftxnno    is '�ⲿ���״���';
comment on column TG_ACCOUNTFLOWO.channelcode is '��������';
comment on column TG_ACCOUNTFLOWO.termcode    is '�ն˴���';
comment on column TG_ACCOUNTFLOWO.accountid   is '�˻�����';
comment on column TG_ACCOUNTFLOWO.accountname is '�˻���';
comment on column TG_ACCOUNTFLOWO.linkaccid   is '�����˻�����';
comment on column TG_ACCOUNTFLOWO.linkaccname is '�����˻���';
comment on column TG_ACCOUNTFLOWO.acccode     is '���˻�����';
comment on column TG_ACCOUNTFLOWO.linkacccode is '�������˻�����';
comment on column TG_ACCOUNTFLOWO.commdate    is 'ͨѶ����';
comment on column TG_ACCOUNTFLOWO.commtime    is 'ͨѶʱ��';
comment on column TG_ACCOUNTFLOWO.transdate   is '��������';
comment on column TG_ACCOUNTFLOWO.transtime   is '����ʱ��';
comment on column TG_ACCOUNTFLOWO.amount      is '���׽��';
comment on column TG_ACCOUNTFLOWO.fee         is '��ѯ��';
comment on column TG_ACCOUNTFLOWO.status      is '����״̬';
comment on column TG_ACCOUNTFLOWO.paycode     is '���㷽ʽ����';
comment on column TG_ACCOUNTFLOWO.txnid       is '�ⲿϵͳ��ˮ��';

alter table TG_ACCOUNTFLOWO
  add constraint PK_TG_ACCOUNTFLOWO primary key (referenceno);


create sequence SEQ_TG_LS
minvalue 1
maxvalue 99999999
start with 1
increment by 1
nocache
cycle;

spool off
