/*
 *    This file is part of acados.
 *
 *    acados is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 3 of the License, or (at your option) any later version.
 *
 *    acados is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with acados; if not, write to the Free Software Foundation,
 *    Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "acados/ocp_nlp/ocp_nlp_cost_ls.h"
#include "acados/ocp_nlp/ocp_nlp_cost_common.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

// blasfeo
#include "blasfeo/include/blasfeo_target.h"
#include "blasfeo/include/blasfeo_common.h"
#include "blasfeo/include/blasfeo_d_aux.h"
#include "blasfeo/include/blasfeo_d_blas.h"
// acados
#include "acados/utils/mem.h"



/* model */

int ocp_nlp_cost_ls_model_calculate_size(void *config_, ocp_nlp_cost_dims *dims)
{
	// extract dims
	int nx = dims->nx;
	int nu = dims->nu;
	int ny = dims->ny;

	int size = 0;

	size += sizeof(ocp_nlp_cost_ls_model);

	size += 1*64;  // blasfeo_mem align

	size += blasfeo_memsize_dmat(ny, ny);  // W
	size += blasfeo_memsize_dmat(nx+nu, ny);  // Cyr
	size += blasfeo_memsize_dvec(ny);  // y_ref

	return size;
}



void *ocp_nlp_cost_ls_model_assign(void *config_, ocp_nlp_cost_dims *dims, void *raw_memory)
{
    char *c_ptr = (char *) raw_memory;

	int nx = dims->nx;
	int nu = dims->nu;
	int ny = dims->ny;

	// struct
    ocp_nlp_cost_ls_model *model = (ocp_nlp_cost_ls_model *) c_ptr;
    c_ptr += sizeof(ocp_nlp_cost_ls_model);

	// blasfeo_mem align
	align_char_to(64, &c_ptr);

	// blasfeo_dmat
	// W
	assign_and_advance_blasfeo_dmat_mem(ny, ny, &model->W, &c_ptr);
	// Cyt
	assign_and_advance_blasfeo_dmat_mem(nx+nu, ny, &model->Cyt, &c_ptr);

	// blasfeo_dvec
	// y_ref
	assign_and_advance_blasfeo_dvec_mem(ny, &model->y_ref, &c_ptr);

	// assert
    assert((char *) raw_memory + ocp_nlp_cost_ls_model_calculate_size(config_, dims) >= c_ptr);

	return model;

}



/* options */

int ocp_nlp_cost_ls_opts_calculate_size(void *config_, ocp_nlp_cost_dims *dims)
{
    int size = 0;

    size += sizeof(ocp_nlp_cost_ls_opts);

    return size;
}



void *ocp_nlp_cost_ls_opts_assign(void *config_, ocp_nlp_cost_dims *dims, void *raw_memory)
{
    char *c_ptr = (char *) raw_memory;

    ocp_nlp_cost_ls_opts *opts = (ocp_nlp_cost_ls_opts *) c_ptr;
    c_ptr += sizeof(ocp_nlp_cost_ls_opts);

    assert((char*)raw_memory + ocp_nlp_cost_ls_opts_calculate_size(config_, dims) >= c_ptr);

    return opts;
}



void ocp_nlp_cost_ls_opts_initialize_default(void *config_, ocp_nlp_cost_dims *dims, void *opts_)
{
	ocp_nlp_cost_ls_opts *opts = opts_;

	opts->gauss_newton_hess = 1;

	return;

}



/* memory */

int ocp_nlp_cost_ls_memory_calculate_size(void *config_, ocp_nlp_cost_dims *dims, void *opts_)
{
	// extract dims
	int nx = dims->nx;
	int nu = dims->nu;
	int ny = dims->ny;

	int size = 0;

    size += sizeof(ocp_nlp_cost_ls_memory);

	size += 1*blasfeo_memsize_dmat(ny, ny);  // W_chol
	size += 1*blasfeo_memsize_dvec(ny);  // res
	size += 1*blasfeo_memsize_dvec(nu+nx);  // grad

	size += 1*64;  // blasfeo_mem align

	return size;
}



void *ocp_nlp_cost_ls_memory_assign(void *config_, ocp_nlp_cost_dims *dims, void *opts_, void *raw_memory)
{
	char *c_ptr = (char *) raw_memory;

	// extract dims
	int nx = dims->nx;
	int nu = dims->nu;
	int ny = dims->ny;

	// struct
    ocp_nlp_cost_ls_memory *memory = (ocp_nlp_cost_ls_memory *) c_ptr;
    c_ptr += sizeof(ocp_nlp_cost_ls_memory);

	// blasfeo_mem align
	align_char_to(64, &c_ptr);

	// W_chol
	assign_and_advance_blasfeo_dmat_mem(ny, ny, &memory->W_chol, &c_ptr);
	// res
	assign_and_advance_blasfeo_dvec_mem(ny, &memory->res, &c_ptr);
	// grad
	assign_and_advance_blasfeo_dvec_mem(nu+nx, &memory->grad, &c_ptr);

    assert((char *) raw_memory + ocp_nlp_cost_ls_memory_calculate_size(config_, dims, opts_) >= c_ptr);

	return memory;
}



struct blasfeo_dvec *ocp_nlp_cost_ls_memory_get_grad_ptr(void *memory_)
{
	ocp_nlp_cost_ls_memory *memory = memory_;

	return &memory->grad;
}



void ocp_nlp_cost_ls_memory_set_RSQrq_ptr(struct blasfeo_dmat *RSQrq, void *memory_)
{
	ocp_nlp_cost_ls_memory *memory = memory_;

	memory->RSQrq = RSQrq;
}



void ocp_nlp_cost_ls_memory_set_ux_ptr(struct blasfeo_dvec *ux, void *memory_)
{
	ocp_nlp_cost_ls_memory *memory = memory_;

	memory->ux = ux;
}



/* workspace */

int ocp_nlp_cost_ls_workspace_calculate_size(void *config_, ocp_nlp_cost_dims *dims, void *opts_)
{
	// extract dims
	int nx = dims->nx;
	int nu = dims->nu;
	int ny = dims->ny;

	int size = 0;

    size += sizeof(ocp_nlp_cost_ls_workspace);

	size += 1*blasfeo_memsize_dmat(nu+nx, ny);  // tmp_nv_ny
	size += 1*blasfeo_memsize_dvec(ny);  // tmp_ny

	size += 1*64;  // blasfeo_mem align

	return size;

}



static void ocp_nlp_cost_ls_cast_workspace(void *config_, ocp_nlp_cost_dims *dims, void *opts_, void *work_)
{
	ocp_nlp_cost_ls_workspace *work = work_;

	// extract dims
	int nx = dims->nx;
	int nu = dims->nu;
	int ny = dims->ny;

    char *c_ptr = (char *) work_;
    c_ptr += sizeof(ocp_nlp_cost_ls_workspace);

	// blasfeo_mem align
	align_char_to(64, &c_ptr);

	// tmp_nv_ny
	assign_and_advance_blasfeo_dmat_mem(nu+nx, ny, &work->tmp_nv_ny, &c_ptr);

	// tmp_ny
	assign_and_advance_blasfeo_dvec_mem(ny, &work->tmp_ny, &c_ptr);

    assert((char *)work + ocp_nlp_cost_ls_workspace_calculate_size(config_, dims, opts_) >= c_ptr);

	return;
}



/* functions */

void ocp_nlp_cost_ls_initialize_qp(void *config_, ocp_nlp_cost_dims *dims, void *model_, void *opts_, void *memory_, void *work_)
{
    ocp_nlp_cost_ls_model *model = model_;
    ocp_nlp_cost_ls_opts *opts = opts_;
    ocp_nlp_cost_ls_memory *memory= memory_;
    ocp_nlp_cost_ls_workspace *work= work_;

	ocp_nlp_cost_ls_cast_workspace(config_, dims, opts_, work_);

	int nx = dims->nx;
	int nu = dims->nu;
	int ny = dims->ny;

	// general Cyt

	// TODO recompute factorization only if W are re-tuned ???
	blasfeo_dpotrf_l(ny, &model->W, 0, 0, &memory->W_chol, 0, 0);

	if (opts->gauss_newton_hess)
	{

		// TODO avoid recomputing the Hessian if both W and Cyt do not change
		blasfeo_dtrmm_rlnn(nu+nx, ny, 1.0, &memory->W_chol, 0, 0, &model->Cyt, 0, 0, &work->tmp_nv_ny, 0, 0);
		blasfeo_dsyrk_ln(nu+nx, ny, 1.0, &work->tmp_nv_ny, 0, 0, &work->tmp_nv_ny, 0, 0, 0.0, memory->RSQrq, 0, 0, memory->RSQrq, 0, 0);

	}
	else
	{

		// TODO exact hessian of ls cost

	}

	return;

}



void ocp_nlp_cost_ls_update_qp_matrices(void *config_, ocp_nlp_cost_dims *dims, void *model_, void *opts_, void *memory_, void *work_)
{

    ocp_nlp_cost_ls_model *model = model_;
    ocp_nlp_cost_ls_opts *opts = opts_;
    ocp_nlp_cost_ls_memory *memory= memory_;
    ocp_nlp_cost_ls_workspace *work= work_;

	ocp_nlp_cost_ls_cast_workspace(config_, dims, opts_, work_);

	int nx = dims->nx;
	int nu = dims->nu;
	int ny = dims->ny;

	if (opts->gauss_newton_hess)
	{

		blasfeo_dgemv_t(nu+nx, ny, 1.0, &model->Cyt, 0, 0, memory->ux, 0, -1.0, &model->y_ref, 0, &memory->res, 0);

		// TODO use lower triangular chol of W to save n_y^2 flops
		blasfeo_dsymv_l(ny, ny, 1.0, &model->W, 0, 0, &memory->res, 0, 0.0, &work->tmp_ny, 0, &work->tmp_ny, 0);
		blasfeo_dgemv_n(nu+nx, ny, 1.0, &model->Cyt, 0, 0, &work->tmp_ny, 0, 0.0, &memory->grad, 0, &memory->grad, 0);

		blasfeo_dtrmm_rlnn(nu+nx, ny, 1.0, &memory->W_chol, 0, 0, &model->Cyt, 0, 0, &work->tmp_nv_ny, 0, 0);
		blasfeo_dsyrk_ln(nu+nx, ny, 1.0, &work->tmp_nv_ny, 0, 0, &work->tmp_nv_ny, 0, 0, 0.0, memory->RSQrq, 0, 0, memory->RSQrq, 0, 0);
	}
	else
	{

		// TODO exact hessian of ls cost

	}

	return;

}



/* config */

void ocp_nlp_cost_ls_config_initialize_default(void *config_)
{
	ocp_nlp_cost_config *config = config_;

	config->model_calculate_size = &ocp_nlp_cost_ls_model_calculate_size;
	config->model_assign = &ocp_nlp_cost_ls_model_assign;
	config->opts_calculate_size = &ocp_nlp_cost_ls_opts_calculate_size;
	config->opts_assign = &ocp_nlp_cost_ls_opts_assign;
	config->opts_initialize_default = &ocp_nlp_cost_ls_opts_initialize_default;
	config->memory_calculate_size = &ocp_nlp_cost_ls_memory_calculate_size;
	config->memory_assign = &ocp_nlp_cost_ls_memory_assign;
	config->memory_get_grad_ptr = &ocp_nlp_cost_ls_memory_get_grad_ptr;
	config->memory_set_ux_ptr = &ocp_nlp_cost_ls_memory_set_ux_ptr;
	config->memory_set_RSQrq_ptr = &ocp_nlp_cost_ls_memory_set_RSQrq_ptr;
	config->workspace_calculate_size = &ocp_nlp_cost_ls_workspace_calculate_size;
	config->initialize_qp = &ocp_nlp_cost_ls_initialize_qp;
	config->update_qp_matrices = &ocp_nlp_cost_ls_update_qp_matrices;
	config->config_initialize_default = &ocp_nlp_cost_ls_config_initialize_default;

	return;

}