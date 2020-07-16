%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%General stuff
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\LoadClass[12pt]{article}

\usepackage{color}
%\usepackage{times}
%\usepackage[T1]{fontenc}
\ifx\pdfoutput\undefined  
%%Running standard latex:
  \usepackage[dvips]{graphicx}
  \DeclareGraphicsExtensions{.eps}
\else
%%Running pdflatex:
  \usepackage[pdftex]{graphicx}
  \DeclareGraphicsExtensions{.jpg,.png,.pdf}
\fi 
\usepackage{epsfig}
\graphicspath{{.}}

\topmargin      -1.0cm
\oddsidemargin  -0.5cm
\evensidemargin -0.5cm
\textheight     21.0cm
\textwidth      17.2cm

\newcommand{\name}{XXX}
\newcommand{\email}{XXX}
\newcommand{\phone}{XXX}
\newcommand{\fax}{XXX}
\newcommand{\sig}{-}

\newcommand\signame{\if\sig-{\name}\else{\sig}\fi}
\newcounter{pageplusone}
\newenvironment{wcletter}{%
	\begin{titlepage}
		\pagestyle{myheadings}
		\setlength{\parindent}{0cm}
		\setlength{\parskip}{\bigskipamount}
		\setcounter{page}{1}
		\setcounter{pageplusone}{2}
}{%
	\end{titlepage}
}
\newcommand{\cut}[1]{}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%letterto 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newcommand{\letterto}[1]{%
	\setlength{\unitlength}{1cm}
	\noindent
	\begin{picture}(14,7)
		%%Title:
		%%Right side header logo:
		\put(0,5.8){\epsfig{figure=wcletter-letterhead201101,width=\textwidth,keepaspectratio}}

		%%Right side sender:
		\put(18.8,4.9){%
			\makebox(0,0)[tr]{%
				\parbox[b]{5\unitlength}{%
					\scriptsize \bf \sf
					\name \\
					\parbox[b]{0.9cm}{Tel.:} \phone \\
					\parbox[b]{0.9cm}{Fax.:} \fax \\
					\parbox[b]{0.9cm}{e-mail:} {\color[rgb]{0,0,1}{\tt \email}}
				}
			}
		}
		%%Date
		\put(18.8,0.5){%
			\makebox(0,0)[tr]{%
				\parbox[b]{5\unitlength}{%
					\normalsize
					\today
				}
			}
		}

		%%Tiny sender label:
		\newfont{\tinysf}{cmss10 scaled 600}
		\put(0.0,4.5){\line(1,0){6}}
		\put(0.0,4.2){\makebox(0,0)[tl]{\parbox[t]{7cm}{ #1 \par }}}
		\put(-2,0){\line(1,0){0.3}}
	\end{picture}\\
}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%letternewpage 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newcommand{\letternewpage}{%
  \vfill
  \mbox{} \hfill /\arabic{pageplusone}
  \addtocounter{pageplusone}{1}
  \newpage
  \setlength{\unitlength}{1cm}
  \noindent
}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%   Goodies
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newcommand{\re}[1]{%
	\vspace{-1.5em}%
	\begin{center}%
	{\bf \underline{Re:~#1}}%
	\end{center}%
	\vspace{-0.5em}%
}

\newcommand{\bye}[1]{%
  \vspace{1cm}
  \par\noindent
  #1 
  \par}

\newcommand{\signature}{%
  \vspace{1cm}
  \begin{flushleft}
	\signame
  \end{flushleft}
}

\newcommand{\ps}[1]{%
  \vspace{1cm}
  {\bf PS:~}\parbox[t]{13cm}{#1 \hfill \mbox{}}
  \vfill}

%%OLD Goodies :-)
\newcommand{\markeditem}[3]{%
  \underline{#1:} \parbox[t]{#2}{#3 \hfill \mbox{}}}

\newenvironment{invpar}%
{\begin{list}{}{\setlength{\itemindent}{-\leftmargini}
      \setlength{\itemsep}{-1mm}}}%
  {\end{list}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
