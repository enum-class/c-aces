#include <Aces.h>
#include <assert.h>
#include <stdio.h>

#define DIM 10

int main()
{
    Aces aces;
    
    /** shared_info **/
    // initialize U
    Coeff u_mem[DIM];
    set_polynomial(&aces.shared_info.pk.u, u_mem, DIM);

    // initialize lambda
    Coeff lambda_mem[DIM][DIM * DIM];
    Matrix2D lambda_mat[DIM];
    for (int i = 0; i < DIM; ++i) {
        lambda_mat[i].dim = DIM;
        lambda_mat[i].data = lambda_mem[i];
    }
    aces.shared_info.pk.lambda.size = DIM;
    aces.shared_info.pk.lambda.data = lambda_mat;

    /** private_key **/
    // initialize x
    Coeff x_mem[DIM][DIM];
    Polynomial x_polies[DIM];
    for (int i = 0; i < DIM; ++i) {
        set_polynomial(&x_polies[i], x_mem[i], DIM);
    }
    aces.private_key.x.size = DIM;
    aces.private_key.x.polies = x_polies;

    // initialize f0
    Coeff f0_mem[DIM][DIM];
    Polynomial f0_polies[DIM];
    for (int i = 0; i < DIM; ++i) {
        set_polynomial(&f0_polies[i], f0_mem[i], DIM);
    }
    aces.private_key.f0.size = DIM;
    aces.private_key.f0.polies = f0_polies;

    // initialize f1
    Coeff f1_mem[DIM];
    set_polynomial(&aces.private_key.f1, f1_mem, DIM);

    init_aces(2, 33, DIM, &aces);

    uint64_t message = 3;
    CipherMessage c_msg;

    Coeff c1_mem[DIM][DIM];
    Polynomial c1_polies[DIM];
    for (int i = 0; i < DIM; ++i) {
        set_polynomial(&c1_polies[i], c1_mem[i], DIM);
    }
    c_msg.c1.size = DIM;
    c_msg.c1.polies = f0_polies;

    Coeff c2_mem[DIM];
    set_polynomial(&c_msg.c2, c2_mem, DIM);

    aces_encrypt(&aces, &message, 1, &c_msg);

    uint64_t recovered_message;
    aces_decrypt(&aces, &c_msg, 1, &recovered_message);

    printf("%lu == %lu\n",recovered_message, message);

    return 0;
}
