//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2011
// European Synchrotron Radiation Facility
// BP 220, Grenoble 38043
// FRANCE
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//###########################################################################
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "HwBufferMgr.h"
#include "FrameBuilder.h"
#include "ThreadUtils.h"
#include "SizeUtils.h"
#include <ostream>

namespace lima
{

class LIBSIMULATOR_API Simulator
{
 public:
	enum Status {
		Ready, Exposure, Readout, Latency,
	};

	Simulator();
	~Simulator();

	HwBufferCtrlObj* getBufferMgr();
	
	void startAcq();
	void stopAcq();

	void setNbFrames(int  nb_frames);
	void getNbFrames(int& nb_frames);

	void setExpTime(double  exp_time);
	void getExpTime(double& exp_time);

	void setLatTime(double  lat_time);
	void getLatTime(double& lat_time);

	void setBin(const Bin& bin);
	void getBin(Bin& bin);
	void checkBin(Bin& bin);

	void setFrameDim(const FrameDim& frame_dim);
	void getFrameDim(FrameDim& frame_dim);
	
	Status getStatus();
	int getNbAcquiredFrames();

	void getMaxImageSize(Size& max_image_size);

	void reset();

 private:
	class SimuThread : public CmdThread
	{
	public:
		enum { // Status
			Ready = MaxThreadStatus, Exposure, Readout, Latency,
		};

		enum { // Cmd 
			StartAcq = MaxThreadCmd, StopAcq,
		};
		
		SimuThread(Simulator& simu);

		virtual void start();
		
		int getNbAcquiredFrames();
		bool m_force_stop;

	protected:
		virtual void init();
		virtual void execCmd(int cmd);
	private:
		void execStartAcq();
		Simulator* m_simu;
		int m_acq_frame_nb;
	};
	friend class SimuThread;

	void init();

	SoftBufferCtrlMgr m_buffer_ctrl_mgr;
	FrameBuilder m_frame_builder;
	double m_exp_time;
	double m_lat_time;
	int m_nb_frames;

	SimuThread m_thread;
};

LIBSIMULATOR_API std::ostream& operator <<(std::ostream& os, Simulator& simu);

} // namespace lima

#endif // SIMULATOR_H
