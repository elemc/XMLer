Name:           XMLer
Version:        0.1
Release:        1%{?dist}
Summary:        The simple XML viewer

License:        GPLv2+
URL:            http://elemc.name
Source0:        http://repo.elemc.name/sources/%{name}-%{version}.tar.xz

BuildRequires:  qt4-devel gcc-c++ cmake desktop-file-utils

%description
The program for storage and information management about passwords

%prep
%setup -q


%build
%cmake
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
%make_install
#desktop-file-install --dir=%{buildroot}/%{_datadir}/applications tools/%{name}.desktop
desktop-file-validate %{buildroot}/%{_datadir}/applications/%{name}.desktop

%files
%defattr(-,root,root)
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor
%{_datadir}/%{name}/translations
#%doc

%changelog
* Thu Nov 01 2012 Alexei Panov <me AT elemc DOT name> 0.1-1
-  initial build
