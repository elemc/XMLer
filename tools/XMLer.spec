%if %{defined suse_version}
    %define cmake /usr/bin/cmake -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_INSTALL_PREFIX:PATH=%{_prefix} -DBUILD_SHARED_LIBS:BOOL=ON -DCMAKE_BUILD_TYPE=Release 
%endif

Name:           XMLer
Version:        0.1
Release:        3%{?dist}
Summary:        The simple XML viewer

License:        GPLv2+
URL:            http://elemc.name
Source0:        http://repo.elemc.name/sources/%{name}-%{version}.tar.xz

BuildRequires:  gcc-c++ cmake desktop-file-utils

%if 0%{?fedora} >= 14 || %{defined suse_version}
BuildRequires:  qt-devel
%else
BuildRequires:  qt4-devel
%endif

%description
The simple XML viewer. Application for view XML files in tree-look. You may search in XML. Set bookmarks. Save XML in another encoding and/or with another formatter.

%prep
%setup -q


%build
%cmake
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
%make_install
desktop-file-validate %{buildroot}/%{_datadir}/applications/%{name}.desktop

%files
%defattr(-,root,root)
%{_bindir}/%{name}
%dir %{_datadir}/%{name}
%dir %{_datadir}/%{name}/translations
%{_datadir}/%{name}/translations/*.qm
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor
%doc README.md

%changelog
* Thu Jan 23 2014 Alexei Panov <me AT elemc DOT name> 0.1-3
-  change SPEC file for OpenSUSE support

* Thu Jan 23 2014 Alexei Panov <me AT elemc DOT name> 0.1-2
-  change minimum cmake version for older distribution

* Thu Nov 01 2012 Alexei Panov <me AT elemc DOT name> 0.1-1
-  initial build
