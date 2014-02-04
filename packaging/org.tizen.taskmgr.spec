Name:       org.tizen.taskmgr
Summary:    Task Manager
Version:    0.14.5
Release:    1
Group:      System/Monitoring
License:    Flora
Source0:    %{name}-%{version}.tar.gz
Source1001: org.tizen.taskmgr.manifest
BuildRequires:  pkgconfig(appcore-efl)
BuildRequires:  pkgconfig(elementary)
BuildRequires:  pkgconfig(aul)
BuildRequires:  pkgconfig(utilX)
BuildRequires:  pkgconfig(rua)
BuildRequires:  pkgconfig(sysman)
BuildRequires:  pkgconfig(ail)
BuildRequires:  pkgconfig(libtzplatform-config)
BuildRequires:  cmake
BuildRequires:  edje-bin
BuildRequires:  embryo-bin
BuildRequires:  gettext-devel
Requires:       tizen-platform-config-tools

%description
Task Manager.

%prep
%setup -q
cp %{SOURCE1001} .

%build
%define PREFIX %{TZ_SYS_RO_APP}"/org.tizen.taskmgr"

cmake . -DCMAKE_INSTALL_PREFIX=%{PREFIX} -DCMAKE_SYS_CONF_DIR=%{TZ_SYS_ETC}

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{TZ_SYS_RO_APP}/org.tizen.taskmgr/bin/*
%{TZ_SYS_RO_APP}/org.tizen.taskmgr/res/*
%attr(-,inhouse,inhouse)
/usr/share/packages/*
/usr/share/icons/default/small/org.tizen.taskmgr.png
%{TZ_SYS_ETC}/smack/accesses.d/org.tizen.taskmgr.rule
